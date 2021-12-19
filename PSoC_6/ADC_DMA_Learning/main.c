/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "stdio.h"
#include "cy_retarget_io.h"


uint16_t adcResult2[3] = {0,0,0};

void SAR_ISR(void)
{

	uint32_t intrMask = Cy_SAR_GetInterruptStatusMasked(pass_0_sar_0_HW);
	if((intrMask & CY_SAR_INTR_EOS) != 0)
	{
		adcResult2[0] = Cy_SAR_GetResult16(pass_0_sar_0_HW, 0);
		adcResult2[1] = Cy_SAR_GetResult16(pass_0_sar_0_HW, 1);
	}
	Cy_SAR_ClearInterrupt(pass_0_sar_0_HW, intrMask);
}

int main(void)
{
    cy_rslt_t result;

    uint32_t ADCResult = 0;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    setvbuf(stdin,NULL,_IONBF,0);

    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                     CY_RETARGET_IO_BAUDRATE);

    printf("\x1b[2J\x1b[;H");
    printf("*************************************************\n\r");
    printf("*                ADC DMA Transfer               *\n\r");
    printf("*************************************************\n\r");


#if 0
    Cy_DMA_Descriptor_Init(&cpuss_0_dw0_0_chan_28_Descriptor_0, &cpuss_0_dw0_0_chan_28_Descriptor_0_config);

    Cy_DMA_Descriptor_SetSrcAddress(&cpuss_0_dw0_0_chan_28_Descriptor_0, SAR->CHAN_RESULT);

    Cy_DMA_Descriptor_SetDstAddress(&cpuss_0_dw0_0_chan_28_Descriptor_0, &ADCResult);

    Cy_DMA_Channel_Init(cpuss_0_dw0_0_chan_28_HW, cpuss_0_dw0_0_chan_28_CHANNEL, &cpuss_0_dw0_0_chan_28_channelConfig);

    Cy_DMA_Channel_Enable(cpuss_0_dw0_0_chan_28_HW, cpuss_0_dw0_0_chan_28_CHANNEL);

    Cy_DMA_Enable(cpuss_0_dw0_0_chan_28_HW);
#else

    Cy_SysAnalog_Init(&pass_0_aref_0_config);

    Cy_SysAnalog_Enable();


    cy_en_sar_status_t status = Cy_SAR_Init(pass_0_sar_0_HW,&pass_0_sar_0_config);

    if(CY_SAR_SUCCESS == status)
    {
    	printf("SAR ADC initialization SUCCESS\r\n");

    	Cy_SAR_Enable(pass_0_sar_0_HW);

    	Cy_SAR_StartConvert(pass_0_sar_0_HW,CY_SAR_START_CONVERT_CONTINUOUS);
    }
    else
    {
    	printf("SAR ADC initialization FAILED\r\n");
    }

	#define sar_intr
	#ifdef sar_intr
		cy_stc_sysint_t SAR0_IrqConfig={
				.intrSrc = pass_0_sar_0_IRQ,
				.intrPriority = 3UL
		};

		cy_en_sysint_status_t sysIntStatus = Cy_SysInt_Init(&SAR0_IrqConfig, SAR_ISR);

		if(sysIntStatus != CY_SYSINT_SUCCESS)
			printf("SAR ADC interrupt init FAILED\r\n");
		else
			printf("SAR ADC interrupt init SUCCESS\r\n");

		NVIC_EnableIRQ(SAR0_IrqConfig.intrSrc);

		Cy_SAR_ClearInterrupt(pass_0_sar_0_HW, CY_SAR_INTR);

	#endif
#endif
    __enable_irq();

    printf("\n\n\r");
    for (;;)
    {
    	if(adcResult2[0])
    	{
        	printf("\rChannel 0 : %f",Cy_SAR_CountsTo_Volts(pass_0_sar_0_HW, 0, adcResult2[0]));
    		adcResult2[0] = 0;
    	}
    	if(adcResult2[1])
    	{
        	printf("\tChannel 1 : %f",Cy_SAR_CountsTo_Volts(pass_0_sar_0_HW, 1, adcResult2[1]));
    		adcResult2[1] = 0;
    	}
    	fflush(stdout);
    	CyDelay(500);
    }
}

/* [] END OF FILE */
