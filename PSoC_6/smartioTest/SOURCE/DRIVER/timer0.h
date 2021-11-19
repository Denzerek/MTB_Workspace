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

#define TIMER0_HW	 			TCPWM0
#define TIMER0_NUM 				0UL
#define TIMER0_MASK 			(1UL << 0)
#define TIMER0_IRQ 				tcpwm_0_interrupts_0_IRQn
#define TIMER0_INTR_PRIORITY	(6U)
#define TIMER0_INPUT_DISABLED 	0x7U

#define TIMER0_PERIOD		10000
#define TIMER0_COMPARE		5000

void Timer_0_Init();
void Timer_TASK();

#endif /* SOURCE_DRIVER_TIMER0_H_ */
