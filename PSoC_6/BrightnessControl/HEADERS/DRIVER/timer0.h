/*
 * timer0.h
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */

#ifndef SOURCE_DRIVER_TIMER0_H_
#define SOURCE_DRIVER_TIMER0_H_

#include "common.h"

#define timer0_print(x)		serialPrint("\r\n[ TIMER0 ] : ");serialPrint(x);

void Timer_0_Init();
void Timer_0_SetPeriod(uint32_t period);
void Timer_0_SetCompare(uint32_t compareVal);
void Timer_TASK();

#endif /* SOURCE_DRIVER_TIMER0_H_ */
