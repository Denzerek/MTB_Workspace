/*
 * LEDDriver.c
 *
 *  Created on: 08-Oct-2021
 *      Author: AustinA
 */

#include "LEDDriver.h"
#include "LED.h"


void powerOnLEDInit()
{
	cyhal_gpio_write(CYBSP_USER_LED1,0);
	cyhal_gpio_write(CYBSP_USER_LED2,0);
	cyhal_gpio_write(CYBSP_LED_RGB_BLUE,0);
	cyhal_gpio_write(CYBSP_LED_RGB_GREEN,0);
	cyhal_gpio_write(CYBSP_LED_RGB_RED,0);
}

void systemLEDsInit()
{
	LEDsInit();
	powerOnLEDInit();
}
