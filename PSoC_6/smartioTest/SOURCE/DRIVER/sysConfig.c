/*
 * sysConfig.c
 *
 *  Created on: 28-Sep-2021
 *      Author: AustinA
 */

#include "sysConfig.h"


void bspInit()
{
	cy_rslt_t result;

	/* Initialize the device and board peripherals */
	result = cybsp_init() ;
	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}

	__enable_irq();

}



void systemInit()
{
	/*Initialize Kit Board Support package*/
	bspInit();

	serialDebugInit();

	systemLEDsInit();

	Timer_0_Init();

	syscfgDriver_print("System Initialized. Executing Application....\r\n");
	syscfgDriver_print("=============================================");
	syscfgDriver_print("============== PWM Brightneess  =============");
	syscfgDriver_print("=============================================");


}
