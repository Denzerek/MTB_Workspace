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
void uart_5_Init(cy_cb_scb_uart_handle_events_t uart_5_Callback);
uint32_t uart_5_receive();

#endif /* DRIVER_UART_H_ */
