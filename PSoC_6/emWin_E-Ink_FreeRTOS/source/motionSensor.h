/*
 * motionSensor.h
 *
 *  Created on: 30-Nov-2021
 *      Author: AustinA
 */

#ifndef SOURCE_MOTIONSENSOR_H_
#define SOURCE_MOTIONSENSOR_H_

#include "common.h"
#include "mtb_bmi160.h"

extern QueueHandle_t motionDataQueue ;
void motion_task();


#endif /* SOURCE_MOTIONSENSOR_H_ */
