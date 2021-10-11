/*
 * LEDDriver.c
 *
 *  Created on: 08-Oct-2021
 *      Author: AustinA
 */

#include "LEDDriver.h"
#include "LED.h"

pwmStruct_s pGreenLED;
pwmStruct_s pRedLED;
pwmStruct_s pBlueLED;



rgbCtrl_s rgbCtrl[RGB_MAX]={
		{&pGreenLED,CYBSP_LED_RGB_GREEN,GREEN},
		{&pRedLED,CYBSP_LED_RGB_RED,RED},
		{&pBlueLED,CYBSP_LED_RGB_BLUE,BLUE}
};


void powerOnLEDInit()
{
	cyhal_gpio_write(CYBSP_USER_LED1,0);
	cyhal_gpio_write(CYBSP_USER_LED2,0);
	cyhal_gpio_write(CYBSP_LED_RGB_BLUE,1);
	cyhal_gpio_write(CYBSP_LED_RGB_GREEN,0);
	cyhal_gpio_write(CYBSP_LED_RGB_RED,1);
}

void luminescenceCtrlInit(pwmStruct_s *ledDimmer,cyhal_gpio_t gpioBase)
{
	ledDimmer->LED_softCounter = 0;
	ledDimmer->LEDtimeout = LED_TIMEOUT_COUNT;
	ledDimmer->ledONDuration = LED_TIMEOUT_COUNT * 2 / 100;
	ledDimmer->percentDC = 2;
	ledDimmer->ledOFFDuration = LED_TIMEOUT_COUNT - (LED_TIMEOUT_COUNT * 20 / 100);
	ledDimmer->GPIO_Base = gpioBase;
}

void systemLEDsInit()
{
	LEDsInit();

	powerOnLEDInit();

	for(int i = 0; i < RGB_MAX ;i++)
	{
		luminescenceCtrlInit(rgbCtrl[i].pwmStruct,rgbCtrl[i].gpioBase);
	}
}

void setRGBTimeout(rgbColor_t rgbColor,uint32_t timeoutValue)
{

	for(int i = 0; i < RGB_MAX ;i++)
	{
		if(rgbCtrl[i].rgbColor == rgbColor)
		{
			SetLEDTimeOutCount(rgbCtrl[i].pwmStruct,timeoutValue);
		}
	}
}

void setRGBDutyCycle(rgbColor_t rgbColor,uint8_t percent)
{

	for(int i = 0; i < RGB_MAX ;i++)
	{
		if(rgbCtrl[i].rgbColor == rgbColor)
		{
			SetLEDDutyCycle(rgbCtrl[i].pwmStruct,percent);
		}
	}

}






void SetLEDTimeOutCount(pwmStruct_s *ledDimmer,uint32_t count)
{
	ledDimmer->LEDtimeout = count;
	ledDimmer->ledONDuration= (uint32_t)(ledDimmer->LEDtimeout * ledDimmer->percentDC / 100);
	sprintf(temp,"LEDtimeout:%d\tledONDuration : %d\tledOFFDuration:%d",ledDimmer->LEDtimeout,ledDimmer->ledONDuration,ledDimmer->ledOFFDuration);
	LEDDriver_print(temp);
}

void SetLEDDutyCycle(pwmStruct_s *ledDimmer,uint8_t percent)
{
	ledDimmer->percentDC = percent;
	ledDimmer->ledONDuration= (uint32_t)(ledDimmer->LEDtimeout * ledDimmer->percentDC / 100);
	ledDimmer->ledOFFDuration = ledDimmer->LEDtimeout - ledDimmer->ledONDuration ;
	sprintf(temp,"LEDtimeout:%d\tledONDuration : %d\tledOFFDuration:%d",ledDimmer->LEDtimeout,ledDimmer->ledONDuration,ledDimmer->ledOFFDuration);
	LEDDriver_print(temp);
}




void LuminesenceRoutine()
{
#if 1
	for(int i = 0; i < RGB_MAX ;i++)
	{
//		if(rgbCtrl[i].pwmStruct == rgbColor)
		{
			if(rgbCtrl[i].pwmStruct->LED_softCounter == rgbCtrl[i].pwmStruct->ledONDuration)
			{
				cyhal_gpio_write(rgbCtrl[i].pwmStruct->GPIO_Base,LED_OFF);
			}


			if(rgbCtrl[i].pwmStruct->LED_softCounter == rgbCtrl[i].pwmStruct->LEDtimeout )
			{
				cyhal_gpio_write(rgbCtrl[i].pwmStruct->GPIO_Base,LED_ON);
				rgbCtrl[i].pwmStruct->LED_softCounter = 0;
			}

			rgbCtrl[i].pwmStruct->LED_softCounter++;
		}
	}
#endif


}














#if 0
void LuminesenceRoutine(pwmStruct_s ledDimmer)
{

	if(ledDimmer.LED_softCounter == ledDimmer.ledONDuration)
	{
		cyhal_gpio_write(ledDimmer.GPIO_Base,LED_OFF);
	}


	if(ledDimmer.LED_softCounter == ledDimmer.LEDtimeout )
	{
		cyhal_gpio_write(ledDimmer.GPIO_Base,LED_ON);
		ledDimmer.LED_softCounter = 0;
	}

	ledDimmer.LED_softCounter++;
}

#endif

#if 0


void LEDTimeOutCount(uint32_t count)
{
	LEDtimeout = count;
	ledONDuration= (uint32_t)(LEDtimeout * percentDC / 100);
	sprintf(temp,"LEDtimeout:%d\tledONDuration : %d\tledOFFDuration:%d",LEDtimeout,ledONDuration,ledOFFDuration);
	LEDDriver_print(temp);
}

void LEDDutyCycle(uint8_t percent)
{
	percentDC = percent;
	ledONDuration= (uint32_t)(LEDtimeout * percent / 100);
		ledOFFDuration = LEDtimeout - ledONDuration ;
	sprintf(temp,"LEDtimeout:%d\tledONDuration : %d\tledOFFDuration:%d",LEDtimeout,ledONDuration,ledOFFDuration);
	LEDDriver_print(temp);
}

void LEDToggleRoutine()
{


	if(LED_softCounter == ledONDuration)
	{
		LED_ALL_SET(LED_OFF);
	}


	if(LED_softCounter == LEDtimeout )
	{
		LED_ALL_SET(LED_ON);
		LED_softCounter = 0;
	}

	LED_softCounter++;
}

#endif
