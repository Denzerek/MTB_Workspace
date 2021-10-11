/*
 * main.h
 *
 *  Created on: 28-Sep-2021
 *      Author: AustinA
 */

#ifndef APPLICATION_COMMON_H_
#define APPLICATION_COMMON_H_


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "stdio.h"
#include "string.h"


//Uncomment the following if RGB
//#define _RGB_GPIO_SCRATCH_MODE_DISABLE

//#define _LED_8_9_GPIO_SCRATCH_MODE_DISABLE


#define UART_TRANSMIT(x)		uart_5_Transmit(x)

#define serialPrint(x)			UART_TRANSMIT(x);

uint8_t temp[50];


#endif /* APPLICATION_COMMON_H_ */
