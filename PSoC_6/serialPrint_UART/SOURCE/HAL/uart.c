/*
 * uart.c
 *
 *  Created on: 08-Oct-2021
 *      Author: AustinA
 */

#include "uart.h"

#define RX_BUFFER_SIZE	50
#define CYBSP_DEBUG_UART_ENABLED 1U
#define CYBSP_DEBUG_UART_HW SCB5
#define CYBSP_DEBUG_UART_IRQ scb_5_interrupt_IRQn

#define peri_0_div_16_0_HW CY_SYSCLK_DIV_16_BIT
#define peri_0_div_16_0_NUM 0U


#define ioss_0_port_5_pin_0_HSIOM P5_0_SCB5_UART_RX
#define ioss_0_port_5_pin_1_HSIOM P5_1_SCB5_UART_TX

#define CYBSP_DEBUG_UART_TX_PORT 		GPIO_PRT5
#define CYBSP_DEBUG_UART_TX_PORT_NUM	CYBSP_DEBUG_UART_TX_PORT
#define CYBSP_DEBUG_UART_TX_PIN 		1U
#define CYBSP_DEBUG_UART_TX_HSIOM 		ioss_0_port_5_pin_1_HSIOM

#define CYBSP_DEBUG_UART_RX_PORT 		GPIO_PRT5
#define CYBSP_DEBUG_UART_RX_PIN 		0U
#define CYBSP_DEBUG_UART_RX_PORT_NUM	CYBSP_DEBUG_UART_RX_PORT
#define CYBSP_DEBUG_UART_RX_HSIOM 		ioss_0_port_5_pin_0_HSIOM


uint8_t Rxbuffer[RX_BUFFER_SIZE];

bool bUartTxCmpltFlag = true;

/* Allocate context for UART operation */
cy_stc_scb_uart_context_t uartContext;


const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_RX_config =
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_HIGHZ,
	.hsiom = CYBSP_DEBUG_UART_RX_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_DEBUG_UART_RX_obj =
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_DEBUG_UART_RX_PORT_NUM,
		.channel_num = CYBSP_DEBUG_UART_RX_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_DEBUG_UART_TX_config =
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG_IN_OFF,
	.hsiom = CYBSP_DEBUG_UART_TX_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_DEBUG_UART_TX_obj =
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_DEBUG_UART_TX_PORT_NUM,
		.channel_num = CYBSP_DEBUG_UART_TX_PIN,
	};
#endif //defined (CY_USING_HAL)


const cy_stc_scb_uart_config_t CYBSP_DEBUG_UART_config =
{
	.uartMode = CY_SCB_UART_STANDARD,
	.enableMutliProcessorMode = false,
	.smartCardRetryOnNack = false,
	.irdaInvertRx = false,
	.irdaEnableLowPowerReceiver = false,
	.oversample = 8,
	.enableMsbFirst = false,
	.dataWidth = 8UL,
	.parity = CY_SCB_UART_PARITY_NONE,
	.stopBits = CY_SCB_UART_STOP_BITS_1,
	.enableInputFilter = false,
	.breakWidth = 11UL,
	.dropOnFrameError = false,
	.dropOnParityError = false,
	.receiverAddress = 0x0UL,
	.receiverAddressMask = 0x0UL,
	.acceptAddrInFifo = false,
	.enableCts = false,
	.ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rtsRxFifoLevel = 0UL,
	.rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
	.rxFifoTriggerLevel = 63UL,
	.rxFifoIntEnableMask = 0UL,
	.txFifoTriggerLevel = 63UL,
	.txFifoIntEnableMask = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_DEBUG_UART_obj =
	{
		.type = CYHAL_RSC_SCB,
		.block_num = 5U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)





void UART_Isr(void)
{
    Cy_SCB_UART_Interrupt(CYBSP_DEBUG_UART_HW, &uartContext);
}


void uartCallback(uint32_t event)
{
	switch(event)
	{
	case CY_SCB_UART_RECEIVE_NOT_EMTPY:

			sprintf(temp,"Received Data : %c\r\n",Cy_SCB_ReadRxFifo(CYBSP_DEBUG_UART_HW));
			serialPrint(temp);
		break;
		case CY_SCB_UART_RECEIVE_DONE_EVENT :
			cyhal_gpio_write(CYBSP_USER_LED2,1);
		break;
		case CY_SCB_UART_TRANSMIT_DONE_EVENT:

			//Toggle the LED when a transmission complete even occurs
			cyhal_gpio_toggle(CYBSP_USER_LED1);

			//Set the transmission complete flag
			bUartTxCmpltFlag = true;
			break;
	}
}

void uart_5_PinsInit()
{
	Cy_GPIO_Pin_Init(CYBSP_DEBUG_UART_RX_PORT, CYBSP_DEBUG_UART_RX_PIN, &CYBSP_DEBUG_UART_RX_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_DEBUG_UART_RX_obj);
#endif //defined (CY_USING_HAL)

	Cy_GPIO_Pin_Init(CYBSP_DEBUG_UART_TX_PORT, CYBSP_DEBUG_UART_TX_PIN, &CYBSP_DEBUG_UART_TX_config);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_DEBUG_UART_TX_obj);
#endif //defined (CY_USING_HAL)
}

void uart_5_ClockInit()
{

#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t peri_0_div_16_0_obj =
	{
		.type = CYHAL_RSC_CLOCK,
		.block_num = peri_0_div_16_0_HW,
		.channel_num = peri_0_div_16_0_NUM,
	};
#endif //defined (CY_USING_HAL)

	Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
	Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0U, 108U);
	Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&peri_0_div_16_0_obj);
#endif //defined (CY_USING_HAL)


	Cy_SysClk_PeriphAssignDivider(PCLK_SCB5_CLOCK, CY_SYSCLK_DIV_16_BIT, 0U);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&CYBSP_DEBUG_UART_obj);
#endif //defined (CY_USING_HAL)

}

void uart_5_InterruptInit()
{

	/* Populate configuration structure (code specific for CM4) */
	cy_stc_sysint_t uartIntrConfig =
	{
	    .intrSrc      = UART_INTR_NUM,
	    .intrPriority = UART_INTR_PRIORITY,
	};

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&uartIntrConfig, &UART_Isr);
	NVIC_EnableIRQ(UART_INTR_NUM);


	Cy_SCB_SetTxInterruptMask(CYBSP_DEBUG_UART_HW,CY_SCB_TX_INTR_UART_DONE);

	Cy_SCB_SetRxInterruptMask(CYBSP_DEBUG_UART_HW,CY_SCB_RX_INTR_NOT_EMPTY);

	Cy_SCB_UART_RegisterCallback(CYBSP_DEBUG_UART_HW,uartCallback,&uartContext);
}

void uart_5_Init()
{
	/*Initialize UART RX and TX pins	 */
	uart_5_PinsInit();

	/*Initiailize UART clock */
	uart_5_ClockInit();

	/* Configure UART to operate */
	(void) Cy_SCB_UART_Init(CYBSP_DEBUG_UART_HW, &CYBSP_DEBUG_UART_config, &uartContext);

	/*Initialize UART Interrupts*/
	uart_5_InterruptInit();

	/* Enable UART to operate */
	Cy_SCB_UART_Enable(CYBSP_DEBUG_UART_HW);

}

void uart_5_Transmit(char* transmitData)
{
	static uint32_t counter = 0;
	if(bUartTxCmpltFlag == false)
	{
		counter++;
		return;
	}
	counter = 0;
	/* Start transmit operation (do not check status) */
	(void) Cy_SCB_UART_Transmit(SCB5, transmitData,strlen(transmitData), &uartContext);

	/*set the falg to false after transmission*/
	bUartTxCmpltFlag = false;
}






#if 0

#include "uart.h"

bool bUartTxCmpltFlag = true;
storageRing_t ringHandler;
storageRingInit(storageRing_t *ringBuff);


/* Allocate context for UART operation */
cy_stc_scb_uart_context_t uartContext;

void UART_Isr(void)
{
    Cy_SCB_UART_Interrupt(SCB5, &uartContext);
    if(0UL == (CY_SCB_UART_TRANSMIT_ACTIVE & Cy_SCB_UART_GetTransmitStatus(SCB5, &uartContext)))
    	bUartTxCmpltFlag = true;
}


void uartInterruptHandling()
{

}

void uart_5_Init()
{

	/* Configure UART to operate */
	(void) Cy_SCB_UART_Init(CYBSP_DEBUG_UART_HW, &CYBSP_DEBUG_UART_config, &uartContext);


	/* Populate configuration structure (code specific for CM4) */
	cy_stc_sysint_t uartIntrConfig =
	{
	    .intrSrc      = UART_INTR_NUM,
	    .intrPriority = UART_INTR_PRIORITY,
	};

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&uartIntrConfig, &UART_Isr);
	NVIC_EnableIRQ(UART_INTR_NUM);

	/* Enable UART to operate */
	Cy_SCB_UART_Enable(SCB5);

	serialPrint("UART Initialized");

}

void uart_5_Transmit(char* transmitData)
{
	static uint32_t counter = 0;
	if(bUartTxCmpltFlag == false)
	{
		counter++;
		return;
	}
	counter = 0;
	/* Start transmit operation (do not check status) */
	(void) Cy_SCB_UART_Transmit(SCB5, transmitData,strlen(transmitData), &uartContext);

	/*set the falg to false after transmission*/
	bUartTxCmpltFlag = false;
}

#endif
