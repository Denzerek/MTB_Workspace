/*
 * LEDDriver.h
 *
 *  Created on: 08-Oct-2021
 *      Author: AustinA
 */

#ifndef DRIVER_LEDDRIVER_H_
#define DRIVER_LEDDRIVER_H_


#include "common.h"

#define LEDDriver_print(x)		serialPrint("\r\n[ LED DRIVER ] : ");serialPrint(x);

void systemLEDsInit();


#endif /* DRIVER_LEDDRIVER_H_ */
