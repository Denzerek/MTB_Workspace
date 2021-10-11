/*
 * sysConfig.h
 *
 *  Created on: 28-Sep-2021
 *      Author: AustinA
 */

#ifndef DRIVER_SYSCONFIG_H_
#define DRIVER_SYSCONFIG_H_



#include "common.h"
#include "uart.h"
#include "timer0.h"
#include "LEDDriver.h"
#include "serialDriver.h"
#include "ringBuffer.h"
#include "command.h"


#define syscfgDriver_print(x)		serialPrint("\r\n[ SYSCFG ] : ");serialPrint(x);



void systemInit();


#endif /* DRIVER_SYSCONFIG_H_ */
