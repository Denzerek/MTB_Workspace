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

	syscfgDriver_print("System Initialized. Executing Application....");
	syscfgDriver_print("=============================================");
	syscfgDriver_print("============== TEMPLATE NAME =============");
	syscfgDriver_print("=============================================");

//	syscfgDriver_print("");
//	syscfgDriver_print("This template code has the followin enabled for ease of use :");
//	syscfgDriver_print("1.Debug uart transmission with queue buffer enable multiple transmission without loss ");
//	syscfgDriver_print("\t2.Debug UART reception with ring buffer handling incomming data. Delimiters are '\r and '\n");
//	syscfgDriver_print("\t3.Command task with feature to added necessary commands and spacer logic to retrieve words from received string");
//	syscfgDriver_print("\t4.A heart beat led to indicate operation controlled by timer 0");


}
