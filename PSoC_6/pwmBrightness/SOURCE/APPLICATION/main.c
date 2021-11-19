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
	{
	    Cy_SmartIO_Init(SMART_IO_9_HW, &SMART_IO_9_config);
	    Cy_SmartIO_Enable(SMART_IO_9_HW);
	}


volatile uint8_t gpio4_prev = 0,gpio4 = 1;
volatile uint8_t gpio3_prev = 0,gpio3 = 1;
volatile uint8_t gpio2_prev = 0,gpio2 = 1;

//Cy_GPIO_Write(TEST_OUT_1_PORT, TEST_OUT_1_PIN,0);

	for (;;)
    {

		Timer_TASK();

		SerialDebug_TASK();

		commandReception_TASK();


		Cy_GPIO_Write(CYBSP_LED_RGB_BLUE_PORT, CYBSP_LED_RGB_BLUE_PIN,1);

		gpio4 = Cy_GPIO_Read(SMART_IN1_PORT, SMART_IN3_PIN);
		gpio3 = Cy_GPIO_Read(SMART_IN2_PORT, SMART_IN2_PIN);
		gpio2 = Cy_GPIO_Read(SMART_IN3_PORT, SMART_IN1_PIN);

		if(gpio2_prev != gpio2)
		{
			if(gpio2)
			{
				Cy_GPIO_Write(CYBSP_LED_RGB_BLUE_PORT, CYBSP_LED_RGB_BLUE_PIN,1);
			}
			else
			{
				Cy_GPIO_Write(CYBSP_LED_RGB_BLUE_PORT, CYBSP_LED_RGB_BLUE_PIN,0);
			}
		}
		if(gpio4_prev != gpio4)
		{
			if(gpio4)
			{
				Cy_GPIO_Write(CYBSP_LED_RGB_GREEN_PORT, CYBSP_LED_RGB_GREEN_PIN,1);
			}
			else
			{
				Cy_GPIO_Write(CYBSP_LED_RGB_GREEN_PORT, CYBSP_LED_RGB_GREEN_PIN,0);
			}
		}
		if(gpio3_prev != gpio3)
		{
			if(gpio3)
			{
				Cy_GPIO_Write(CYBSP_LED_RGB_RED_PORT, CYBSP_LED_RGB_RED_PIN,1);
			}
			else
			{
				Cy_GPIO_Write(CYBSP_LED_RGB_RED_PORT, CYBSP_LED_RGB_RED_PIN,0);
			}
		}


		gpio4_prev = gpio4;
		gpio3_prev = gpio3;
		gpio2_prev = gpio2;


#if 0
		gpio4 = Cy_GPIO_Read(SMART_IN1_PORT, SMART_IN3_PIN);
		gpio3 = Cy_GPIO_Read(SMART_IN2_PORT, SMART_IN2_PIN);
		gpio2 = Cy_GPIO_Read(SMART_IN3_PORT, SMART_IN1_PIN);


		if(gpio2_prev != gpio2)
		{
			if(gpio2)
			{
				sprintf(temp,"port 1 pin 2 is ON %d",gpio2);
				main_print(temp);
			}
			else
			{
				sprintf(temp,"port 1 pin 2 is OFF %d",gpio2);
				main_print(temp);
			}
		}
		if(gpio4_prev != gpio4)
		{
			if(gpio4)
			{
				sprintf(temp,"port 1 pin 4 is ON %d",gpio4);
				main_print(temp);
			}
			else
			{
				sprintf(temp,"port 1 pin 4 is OFF %d",gpio4);
				main_print(temp);
			}
		}
		if(gpio3_prev != gpio3)
		{
			if(gpio3)
			{
				sprintf(temp,"port 1 pin 3 is ON %d",gpio3);
				main_print(temp);
			}
			else
			{
				sprintf(temp,"port 1 pin 3 is OFF %d\r\n",gpio3);
				main_print(temp);
			}
		}


		gpio4_prev = gpio4;
		gpio3_prev = gpio3;
		gpio2_prev = gpio2;
#endif
    }
}

/* [] END OF FILE */
