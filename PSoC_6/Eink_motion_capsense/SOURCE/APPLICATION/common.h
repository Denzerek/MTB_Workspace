/*
 * main.h
 *
 *  Created on: 28-Sep-2021
 *      Author: AustinA
 */

#ifndef APPLICATION_COMMON_H_
#define APPLICATION_COMMON_H_


#define _FREERTOS_ENABLED


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "stdio.h"
#include "string.h"

//FreeRTOS Headers
#ifdef _FREERTOS_ENABLED
#include <FreeRTOS.h>
#include <task.h>
#include "semphr.h"
#endif


//Uncomment the following if RGB
#define _RGB_GPIO_SCRATCH_MODE_DISABLE

//#define _LED_8_9_GPIO_SCRATCH_MODE_DISABLE


#define UART_TRANSMIT(x)		serialDebugTransmit(x)

#define serialPrint(x)			UART_TRANSMIT(x);

extern uint8_t temp[200];


#endif /* APPLICATION_COMMON_H_ */
