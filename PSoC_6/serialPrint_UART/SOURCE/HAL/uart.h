/*
 * uart.h
 *
 *  Created on: 08-Oct-2021
 *      Author: AustinA
 */

#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_

#include "common.h"

/* Assign UART interrupt number and priority */
#define UART_INTR_NUM        ((IRQn_Type) scb_5_interrupt_IRQn)
#define UART_INTR_PRIORITY   (7U)


void uart_5_Transmit(char* transmitData);
void uart_5_Init();

#endif /* DRIVER_UART_H_ */
