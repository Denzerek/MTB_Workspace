/*
 * timer0.c
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */

#include "timer0.h"

#define TIMER0_HW	 			TCPWM0
#define TIMER0_NUM 				0UL
#define TIMER0_MASK 			(1UL << 0)
#define TIMER0_IRQ 				tcpwm_0_interrupts_0_IRQn
#define TIMER0_INTR_PRIORITY	(6U)
#define TIMER0_INPUT_DISABLED 	0x7U

#define TIMER0_PERIOD		1000
#define TIMER0_COMPARE		500


const cy_stc_tcpwm_counter_config_t TIMER0_config =
{
	.period = TIMER0_PERIOD,
	.clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
	.runMode = CY_TCPWM_COUNTER_CONTINUOUS,
	.countDirection = CY_TCPWM_COUNTER_COUNT_UP,
	.compareOrCapture = CY_TCPWM_COUNTER_MODE_COMPARE,
	.compare0 = TIMER0_COMPARE,
	.compare1 = 16384,
	.enableCompareSwap = false,
	.interruptSources = CY_TCPWM_INT_ON_CC,
	.captureInputMode = TIMER0_INPUT_DISABLED & 0x3U,
	.captureInput = CY_TCPWM_INPUT_0,
	.reloadInputMode = TIMER0_INPUT_DISABLED & 0x3U,
	.reloadInput = CY_TCPWM_INPUT_0,
	.startInputMode = TIMER0_INPUT_DISABLED & 0x3U,
	.startInput = CY_TCPWM_INPUT_0,
	.stopInputMode = TIMER0_INPUT_DISABLED & 0x3U,
	.stopInput = CY_TCPWM_INPUT_0,
	.countInputMode = TIMER0_INPUT_DISABLED & 0x3U,
	.countInput = CY_TCPWM_INPUT_1,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t TIMER0_obj =
	{
		.type = CYHAL_RSC_TCPWM,
		.block_num = 0U,
		.channel_num = 0U,
	};
#endif //defined (CY_USING_HAL)



#define TIMER0_CLK_HW CY_SYSCLK_DIV_16_BIT
#define TIMER0_CLK_NUM 1U

#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t TIMER0_CLK_obj =
	{
		.type = CYHAL_RSC_CLOCK,
		.block_num = TIMER0_CLK_HW,
		.channel_num = TIMER0_CLK_NUM,
	};
#endif //defined (CY_USING_HAL)



void Timer_0_ClockInit(void)
{

	Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 1U);
	Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 1U, 1U);
	Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 1U);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&TIMER0_CLK_obj);
#endif //defined (CY_USING_HAL)

	Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM0_CLOCKS0, CY_SYSCLK_DIV_16_BIT, 1U);
#if defined (CY_USING_HAL)
	cyhal_hwmgr_reserve(&TIMER0_obj);
#endif //defined (CY_USING_HAL)
}

bool compareTimeOutFlag = false;
void timer_0_ISR()
{

    uint32_t interrupts = Cy_TCPWM_GetInterruptStatus(TIMER0_HW, TIMER0_NUM);
    /* Now the 'interrupts' contains all the currently pending interrupt masks */
    if(0UL != (CY_TCPWM_INT_ON_CC & interrupts))
    {
//    	cyhal_gpio_toggle(CYBSP_USER_LED2);
//		cyhal_gpio_toggle(CYBSP_LED_RGB_GREEN);
//    	sprintf(temp,"%d\r\n",count++);
//    	timer0_print(temp);
    	compareTimeOutFlag = true;

    }
    Cy_TCPWM_ClearInterrupt(TIMER0_HW, TIMER0_NUM,CY_TCPWM_INT_ON_CC);
}

#define LED_TIMEOUT_COUNT	30
static uint32_t LED_softCounter = 0;
#include "LEDDriver.h"

void Timer_TASK()
{
	if(compareTimeOutFlag == true)
	{
		compareTimeOutFlag = false;
		LuminesenceRoutine();
	}
}

void Timer_0_InterruptInit()
{

	/* Populate configuration structure (code specific for CM4) */
	cy_stc_sysint_t timer0IntrConfig =
	{
	    .intrSrc      = TIMER0_IRQ,
	    .intrPriority = TIMER0_INTR_PRIORITY,
	};

	/* Hook interrupt service routine and enable interrupt */
	(void) Cy_SysInt_Init(&timer0IntrConfig, &timer_0_ISR);
	NVIC_EnableIRQ(TIMER0_IRQ);
}


void Timer_0_SetPeriod(uint32_t period)
{
	sprintf(temp,"Current PERIOD : %d",Cy_TCPWM_Counter_GetPeriod(TIMER0_HW, TIMER0_NUM));
	timer0_print(temp);
	Cy_TCPWM_Counter_SetPeriod(TIMER0_HW, TIMER0_NUM,period);
}


void Timer_0_SetCompare(uint32_t compareVal)
{
	sprintf(temp,"Current CompareVal : %d",  Cy_TCPWM_Counter_GetCompare0Val(TIMER0_HW, TIMER0_NUM));
	timer0_print(temp);
	Cy_TCPWM_Counter_SetCompare0Val(TIMER0_HW, TIMER0_NUM,compareVal);
}


void Timer_0_Init()
{
	Timer_0_ClockInit();

   if (CY_TCPWM_SUCCESS != Cy_TCPWM_Counter_Init(TIMER0_HW, TIMER0_NUM, &TIMER0_config))
   {
       /* Handle possible errors */
	   timer0_print("Timer 0 Initialization\r\n");
	   return;
   }


   timer0_print("Timer 0 Initialized\r\n");

   /* Enable the Interrupts */
   Timer_0_InterruptInit();

   /* Enable the initialized counter */
   Cy_TCPWM_Counter_Enable(TIMER0_HW, TIMER0_NUM);

   /* Then start the counter */
   Cy_TCPWM_TriggerStart_Single(TIMER0_HW, TIMER0_NUM);

//   timer0_print("[ SUCCESS ] : Timer 0 Started\r\n");
}
