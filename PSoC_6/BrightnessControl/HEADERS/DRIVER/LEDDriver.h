/*
 * LEDDriver.h
 *
 *  Created on: 08-Oct-2021
 *      Author: AustinA
 */

#ifndef DRIVER_LEDDRIVER_H_
#define DRIVER_LEDDRIVER_H_


#include "common.h"

#define LED_ON	0
#define LED_OFF	1
#define LED_ALL_SET(x) 		cyhal_gpio_write(CYBSP_LED_RGB_GREEN,x);cyhal_gpio_write(CYBSP_USER_LED1,x);cyhal_gpio_write(CYBSP_USER_LED2,x);
#define LED_TIMEOUT_COUNT	100

#define GREEN_LED_SET(x)	cyhal_gpio_write(CYBSP_LED_RGB_GREEN,x)
#define RED_LED_SET(x)		cyhal_gpio_write(CYBSP_LED_RGB_RED,x)
#define BLUE_LED_SET(x)		cyhal_gpio_write(CYBSP_LED_RGB_BLUE,x)
#define USER1_LED_SET(x)		cyhal_gpio_write(CYBSP_USER_LED1,x)
#define USER2_LED_SET(x)		cyhal_gpio_write(CYBSP_USER_LED2,x)
#define LEDDriver_print(x)	serialPrint("\r\n[ LED DRIVER] : ");serialPrint(x);

typedef struct{
	uint32_t LED_softCounter;
	uint32_t LEDtimeout ;
	uint32_t ledONDuration;
	uint8_t percentDC ;
	uint32_t ledOFFDuration;
	cyhal_gpio_t GPIO_Base;
}pwmStruct_s;


typedef enum{
	RED,
	BLUE,
	GREEN,
	RGB_MAX
}rgbColor_t;

typedef struct{
	pwmStruct_s	*pwmStruct;
	cyhal_gpio_t gpioBase;
	rgbColor_t	rgbColor;
}rgbCtrl_s;


void setRGBDutyCycle(rgbColor_t rgbColor,uint8_t percent);
void setRGBTimeout(rgbColor_t rgbColor,uint32_t timeoutValue);

void LuminesenceRoutine();
void SetLEDDutyCycle(pwmStruct_s *ledDimmer,uint8_t percent);
void SetLEDTimeOutCount(pwmStruct_s *ledDimmer,uint32_t count);
void luminescenceCtrlInit(pwmStruct_s *ledDimmer,cyhal_gpio_t gpioBase);
void systemLEDsInit();


#endif /* DRIVER_LEDDRIVER_H_ */
