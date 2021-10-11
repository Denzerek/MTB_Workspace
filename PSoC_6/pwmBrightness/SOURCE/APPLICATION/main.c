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

#include "sysConfig.h"

int main(void)
{
	/*System Initialization for peripherals*/
	systemInit();


//	void smartIOInit()
//	{
//	    Cy_SmartIO_Init(SMART_PORT_IO_HW, &SMART_IO_PORT_config);
//	    Cy_SmartIO_Enable(SMART_IO_PORT_HW);
//	}


volatile uint8_t gpio4_prev = 0,gpio4 = 1;
volatile uint8_t gpio5_prev = 0,gpio5 = 1;
volatile uint8_t gpio6_prev = 0,gpio6 = 1;
Cy_GPIO_Write(SMART_IN1_PORT, SMART_IN1_PIN,1);
uint32_t counter = 0;
	for (;;)
    {

		Timer_TASK();

		SerialDebug_TASK();

		commandReception_TASK();


		gpio4 = Cy_GPIO_Read(SMART_IN1_PORT, SMART_IN1_PIN);
		gpio5 = Cy_GPIO_Read(SMART_IN2_PORT, SMART_IN2_PIN);
		gpio6 = Cy_GPIO_Read(SMART_IN3_PORT, SMART_IN3_PIN);

		if(gpio6_prev != gpio6)
		{
			if(gpio6)
			{
				sprintf(temp,"port 5 pin 6 is ON %d",gpio6);
				main_print(temp);
			}
			else
			{
				sprintf(temp,"port 5 pin 6 is OFF %d",gpio6);
				main_print(temp);
			}
		}
		if(gpio4_prev != gpio4)
		{
			if(gpio4)
			{
				sprintf(temp,"port 5 pin 4 is ON %d",gpio4);
				main_print(temp);
			}
			else
			{
				sprintf(temp,"port 5 pin 4 is OFF %d",gpio4);
				main_print(temp);
			}
		}
		if(gpio5_prev != gpio5)
		{
			if(gpio5)
			{
				sprintf(temp,"port 5 pin 5 is ON %d",gpio5);
				main_print(temp);
			}
			else
			{
				sprintf(temp,"port 5 pin 5 is OFF %d\r\n",gpio5);
				main_print(temp);
			}
		}


		gpio4_prev = gpio4;
		gpio5_prev = gpio5;
		gpio6_prev = gpio6;

    }
}

/* [] END OF FILE */
