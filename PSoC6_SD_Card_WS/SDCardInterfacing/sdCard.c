/*
 * sdCard.c
 *
 *  Created on: 19-Feb-2022
 *      Author: denzo
 */
#include "sdCard.h"



/* Assign SDHC_1_HW interrupt number and priority */
#define SD_Host_INTR_NUM        sdhc_1_interrupt_general_IRQn
#define SD_Host_INTR_PRIORITY   (3UL)


///* Then start the counter */
#define CARD_DETECT_TIMER_RUN()		Cy_TCPWM_TriggerStart_Single(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM)
//#define CARD_DETECT_TIMER_GET_COUNT()	Cy_TCPWM_Counter_GetCounter(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM)
//#define CARD_DETECT_TIMER_PERIOD()	CARD_DETECT_TIMER_config.compare0
//#define CARD_DETECT_DEBOUNCE	(CARD_DETECT_TIMER_GET_COUNT() - CARD_DETECT_TIMER_PERIOD())

/* Allocate context for SD Host operation */
cy_stc_sd_host_context_t sdHostContext;

volatile static uint8_t cardDetectFlag;
volatile static sdCardProcess_t cardStatus ;
volatile static uint8_t cardDetectTimerExpiredStatus = 1;

/* Populate configuration structure (code specific for CM4) */
const cy_stc_sysint_t sdHostIntrConfig =
{
            #if (CY_CPU_CORTEX_M0P)
                 /* .intrSrc */ NvicMux4_IRQn,
                /* .cm0pSrc */ SD_Host_INTR_NUM,
            #else
                 /* .intrSrc */ SD_Host_INTR_NUM, /* SD Host interrupt number (non M0 core)*/
            #endif
        /* .intrPriority */ SD_Host_INTR_PRIORITY
};



/* Populate configuration structure (code specific for CM4) */
const cy_stc_sysint_t sdHostCardDetectIntrConfig =
{
		 /* .intrSrc */ SDHC1_CARD_DETECT_N_NUM_IRQ,
        /* .intrPriority */ SD_Host_INTR_PRIORITY
};


/* Populate configuration structure (code specific for CM4) */
const cy_stc_sysint_t cardDetectTimerIntr =
{
		 /* .intrSrc */ CARD_DETECT_TIMER_IRQ,
        /* .intrPriority */ SD_Host_INTR_PRIORITY
};



sdCardProcess_t sdCardProcessState = CARD_ABSENT;

void cardDetectTimer_ISR()
{
	uint32_t interrupts = Cy_TCPWM_GetInterruptStatusMasked(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM);
	if (0UL != (CY_TCPWM_INT_ON_TC & interrupts))
	{
		/* Handle the Terminal Count event */
		printf("Timer interrupt TC\r\n");
		sdCardProcessState = CARD_INSERT_CHECK;
		cardDetectTimerExpiredStatus = 1;
	}
	/* Clear the interrupt */
	Cy_TCPWM_ClearInterrupt(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM, interrupts);
}

void SD_Host_CardDetect_Isr()
{
	sdCardProcessState = CARD_DETECT_ACTIVITY;
	cardDetectFlag = 1;
	CARD_DETECT_TIMER_RUN();
	Cy_GPIO_ClearInterrupt(SDHC1_CARD_DETECT_N_NUM_PORT, SDHC1_CARD_DETECT_N_NUM_PIN);
}

void cardDetectTimerInit()
{
    if (CY_TCPWM_SUCCESS != Cy_TCPWM_Counter_Init(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM, &CARD_DETECT_TIMER_config))
    {
        /* Handle possible errors */
    }


	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&cardDetectTimerIntr, &cardDetectTimer_ISR);
	#if (CY_CPU_CORTEX_M0P)
		NVIC_EnableIRQ(NvicMux4_IRQn);
	#else
		NVIC_EnableIRQ(cardDetectTimerIntr.intrSrc);
	#endif

    /* Enable the initialized counter */
    Cy_TCPWM_Counter_Enable(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM);
}





void sdCardProcess()
{
	switch(sdCardProcessState)
	{
		case CARD_ABSENT:
			cardStatus = CARD_ABSENT;
			break;
		case CARD_DETECT_ACTIVITY:
			printf("[ SD ] :\r\n\n");
			printf("[ SD ] : CARD_DETECT_ACTIVITY\r\n");
			CARD_DETECT_TIMER_RUN();
			sdCardProcessState = CARD_DETECT_DEBOUNC_WAIT;
			printf("[ SD ] : CARD_DETECT_DEBOUNC_WAIT\r\n");
			break;
		case CARD_DETECT_DEBOUNC_WAIT:
//			printf(" %d \r\n",Cy_TCPWM_Counter_GetCounter(CARD_DETECT_TIMER_HW, CARD_DETECT_TIMER_NUM));
			break;
		case CARD_INSERT_CHECK:
			printf("[ SD ] : CARD_INSERT_CHECK\r\n");
			if(Cy_GPIO_Read(SDHC1_CARD_DETECT_N_NUM_PORT, SDHC1_CARD_DETECT_N_NUM_PIN))
			{
				sdCardProcessState = CARD_ABSENT ;
				printf("[ SD ] : CARD_ABSENT\r\n");
			}
			else
			{
				sdCardProcessState = CARD_INITIALIZE;
			}
			break;
		case CARD_INITIALIZE:
			printf("[ SD ] : CARD_INITIALIZE\r\n");
			if(cardInit() == CY_SD_HOST_SUCCESS)
			{
				sdCardProcessState = CARD_INSERTED_INITIALIZED;
				printf("[ SD ] : CARD_INSERTED_INITIALIZED\r\n");
			}
			else
			{
				sdCardProcessState = CARD_ABSENT;
				printf("[ SD ] : CARD_ABSENT\r\n");
			}
			break;
		case CARD_INSERTED_INITIALIZED:
			cardStatus = CARD_INSERTED_INITIALIZED;
			break;
	}
}










void checkCardStatus()
{
	if(cardDetectFlag  && cardDetectTimerExpiredStatus)
	{
		cardDetectFlag = 0;
		cardDetectTimerExpiredStatus = 0;

		if(Cy_GPIO_Read(SDHC1_CARD_DETECT_N_NUM_PORT, SDHC1_CARD_DETECT_N_NUM_PIN))
		{
			printf("[ SD ] : Card Removed\r\n");
			cardStatus = 0;
		}
		else
		{
			printf("[ SD ] : Card Inserted\r\n");
			if(cardInit() == CY_SD_HOST_SUCCESS)
			{
				cardStatus = 1;
			}
			else
			{
				cardStatus = 0;
				printf("In error\r\n");
			}
		}
	}

	if(cardStatus)
	{

		/* Note: SD Host and the card must be initialized in ADMA2 or SDMA
		 * mode before using the code below.
		 */
		#define DATA       (7U)  /* Data to write. */
		cy_stc_sd_host_write_read_config_t data;
		cy_en_sd_host_status_t ret;
		uint8_t rxBuff[CY_SD_HOST_BLOCK_SIZE];   /* Data to read. */
		uint8_t txBuff[CY_SD_HOST_BLOCK_SIZE];   /* Data to write. */
		memset(txBuff, 123, sizeof(txBuff));  /* Fill the array with data to write. */
		data.address = 0UL;         /* The address to write/read data on the card or eMMC. */
		data.numberOfBlocks = 1UL;  /* The number of blocks to write/read (Single block write/read). */
		data.autoCommand = CY_SD_HOST_AUTO_CMD_NONE;  /* Selects which auto commands are used if any. */
		data.dataTimeout = 12UL;     /* The timeout value for the transfer. */
		data.enReliableWrite = false; /* For EMMC cards enable reliable write. */
		data.enableDma = true;  /* Enable DMA mode. */
		data.data = (uint32_t*)txBuff;  /* The pointer to data to write. */
		printf("Starting Write operation \r\n");
		ret = Cy_SD_Host_Write(SDHC1, &data, &sdHostContext);  /* Write data to the card. */
		if (CY_SD_HOST_SUCCESS == ret)
		{
		    while (CY_SD_HOST_XFER_COMPLETE != (Cy_SD_Host_GetNormalInterruptStatus(SDHC1) & CY_SD_HOST_XFER_COMPLETE))
		    {
		        /* Wait for the data-transaction complete event. */
		    }
		}
		printf("SD card write operation complete\r\n");
		printf("Starting read operation \r\n");
		data.data = (uint32_t*)rxBuff;  /* The pointer to data to read. */
		ret = Cy_SD_Host_Read(SDHC1, &data, &sdHostContext);   /* Read data from the card. */
		if (CY_SD_HOST_SUCCESS == ret)
		{
		    while (CY_SD_HOST_XFER_COMPLETE != (Cy_SD_Host_GetNormalInterruptStatus(SDHC1) & CY_SD_HOST_XFER_COMPLETE))
		    {
		        /* Wait for the data-transaction complete event. */
		    }

		    /* Clear the data-transaction complete event. */
		    Cy_SD_Host_ClearNormalInterruptStatus(SDHC1, CY_SD_HOST_XFER_COMPLETE);
		}

		printf("Read operation complete\r\n");
		printf("Data Read Back :");
		for(int i=0;i<CY_SD_HOST_BLOCK_SIZE;i++)
		{
			printf("%d ",rxBuff[i]);
		}
		printf("\r\n\n");
		CyDelay(2000);
	}

#if 0
	uint32_t count;
	printf("CARD_DETECT_DEBOUNCE %d\r\n",CARD_DETECT_DEBOUNCE);
	CyDelay(100);
#endif
}

bool Cy_SD_Host_IsCardConnected(SDHC_Type const * 	base	)
{
	return true;
}


void SD_Host_User_Isr(void)
{
    volatile uint32_t normalStatus;
    volatile uint32_t errorStatus;

    normalStatus = Cy_SD_Host_GetNormalInterruptStatus(SDHC_1_HW);

    /* Check the Error event */
    switch(normalStatus)
    {
		case CY_SD_HOST_CARD_INSERTION:
			printf("[ SD NORMAL INT] : SD Card Inserted\r\n");
			break;
		case CY_SD_HOST_CARD_REMOVAL:
			printf("[ SD NORMAL INT] : SD Card Removed\r\n");
			break;
		default:
			printf("[ SD NORMAL INT] : Unknown Interrupt %d\r\n",normalStatus);

			break;
    }

    if (0u < normalStatus)
    {
        /* Clear the normalStatus event */
        Cy_SD_Host_ClearNormalInterruptStatus(SDHC_1_HW, normalStatus);
    }

    errorStatus = Cy_SD_Host_GetErrorInterruptStatus(SDHC_1_HW);
    /* Check the Error event */
    switch(errorStatus)
   {
		default:
			printf("[ SD ERROR INT] : Unknown Interrupt %ud\r\n",errorStatus);
			break;
   }
    if (0u < errorStatus)
    {
        /* Clear the Error event */
        Cy_SD_Host_ClearErrorInterruptStatus(SDHC_1_HW, errorStatus);
    }

    /* Add the use code here. */
}

void SD_Host_Init()
{
	cy_en_sd_host_status_t status;

	 Cy_SD_Host_Enable(SDHC_1_HW);

	 /* Generate an interrupt on SD card insertion or removal */
	 Cy_SD_Host_SetNormalInterruptMask(SDHC1, (CY_SD_HOST_CARD_INSERTION | CY_SD_HOST_CARD_REMOVAL));

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&sdHostCardDetectIntrConfig, &SD_Host_CardDetect_Isr);
	#if (CY_CPU_CORTEX_M0P)
		NVIC_EnableIRQ(NvicMux4_IRQn);
	#else
		NVIC_EnableIRQ(sdHostCardDetectIntrConfig.intrSrc);
	#endif

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&sdHostIntrConfig, &SD_Host_User_Isr);
	#if (CY_CPU_CORTEX_M0P)
		NVIC_EnableIRQ(NvicMux4_IRQn);
	#else
		NVIC_EnableIRQ(sdHostIntrConfig.intrSrc);
	#endif


		printf("[ SD ] : Initializing \r\n");
	/* Configure SD Host to operate */
	status = Cy_SD_Host_Init(SDHC_1_HW, &SDHC_1_config, &sdHostContext);

	if(status != CY_SD_HOST_SUCCESS)
	{
		printf("[ SD ] : SD Host Init Failure %d\r\n",status);
		return;
	}

	cardDetectTimerInit();


	sdCardProcessState=CARD_INSERT_CHECK;


}



void decode(cy_en_sd_host_status_t status)
{

	if((status & CY_SD_HOST_ERROR)==CY_SD_HOST_ERROR)
	{
		printf("CY_SD_HOST_ERROR \r\n");
	}
	if((status & CY_SD_HOST_ERROR_INVALID_PARAMETER)==CY_SD_HOST_ERROR_INVALID_PARAMETER)
	{
		printf("CY_SD_HOST_ERROR_INVALID_PARAMETER \r\n");

	}
	if((status & CY_SD_HOST_ERROR_OPERATION_IN_PROGRESS)==CY_SD_HOST_ERROR_OPERATION_IN_PROGRESS)
	{
		printf("CY_SD_HOST_ERROR_OPERATION_IN_PROGRESS \r\n");

	}
	if((status & CY_SD_HOST_ERROR_UNINITIALIZED)==CY_SD_HOST_ERROR_UNINITIALIZED)
	{
		printf("CY_SD_HOST_ERROR_UNINITIALIZED \r\n");

	}
	if((status & CY_SD_HOST_ERROR_TIMEOUT)==CY_SD_HOST_ERROR_TIMEOUT)
	{
		printf("CY_SD_HOST_ERROR_TIMEOUT \r\n");
	}
	if((status & CY_SD_HOST_OPERATION_INPROGRESS)==CY_SD_HOST_OPERATION_INPROGRESS)
	{
		printf("CY_SD_HOST_OPERATION_INPROGRESS \r\n");
	}
	if((status & CY_SD_HOST_ERROR_UNUSABLE_CARD)==CY_SD_HOST_ERROR_UNUSABLE_CARD)
	{
		printf("CY_SD_HOST_ERROR_UNUSABLE_CARD \r\n");
	}
	if((status & CY_SD_HOST_ERROR_DISCONNECTED)== CY_SD_HOST_ERROR_DISCONNECTED)
	{
		printf("CY_SD_HOST_ERROR_DISCONNECTED \r\n");
	}
}


cy_en_sd_host_status_t cardInit()
{
	cy_en_sd_host_status_t status;

	/* Initialize the card */
	status = Cy_SD_Host_InitCard(SDHC_1_HW, &SDHC_1_card_cfg, &sdHostContext);
	if(status != CY_SD_HOST_SUCCESS)
	{
		printf("[ SD ] : SDCard initialization Error %d \r\n",status);
		decode(status);
		return status;
	}


	SD_printSDHC_1_cardType();
	SD_printSDHC_1_cardCapacity();
	printf("[ SD ] : SDCard initialized\r\n");
	return status;



}



void SD_printSDHC_1_cardType()
{
	printf("[ SD ] : Card Type : ");
	switch(*SDHC_1_card_cfg.cardType)
	{
		case CY_SD_HOST_SD:
			printf(" The Secure Digital card (SD).\r\n");break;
		case CY_SD_HOST_SDIO:
			printf(" The CD Input Output card (SDIO).\r\n");break;
		case CY_SD_HOST_EMMC:
			printf(" The Embedded Multimedia card (eMMC).\r\n");break;
		case CY_SD_HOST_COMBO:
			printf(" The Combo card (SD + SDIO).\r\n");break;
		case CY_SD_HOST_UNUSABLE:
			printf(" The unusable or not supported.\r\n");break;
		case CY_SD_HOST_NOT_EMMC:
			printf(" The card is not eMMC.\r\n");break;
	}
}

void SD_printSDHC_1_cardCapacity()
{
	printf("[ SD ] : Card Capacity : ");
	switch(*SDHC_1_card_cfg.cardCapacity)
	{
		case CY_SD_HOST_SDSC:
			printf(" SDSC - Secure Digital Standard Capacity (up to 2 GB).\r\n");
				break;
	//		case CY_SD_HOST_SDHC:
	//			printf(" SDHC - Secure Digital High Capacity (up to 32 GB).\r\n");break;
	//		case CY_SD_HOST_EMMC_LESS_2G:
	//			printf(" The eMMC block addressing for less than 2GB.\r\n");break;
		case CY_SD_HOST_EMMC_GREATER_2G:
			printf(" The eMMC block addressing for greater than 2GB.\r\n");break;
		case CY_SD_HOST_SDXC:
			printf(" SDXC - Secure Digital Extended Capacity (up to 2 TB).\r\n");break;
		case CY_SD_HOST_UNSUPPORTED:
			printf(" Not supported.\r\n");break;
	}
}


