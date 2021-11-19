/*
 * command.h
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */

#ifndef HEADERS_APPLICATION_COMMAND_H_
#define HEADERS_APPLICATION_COMMAND_H_


#include "common.h"
#include "serialDriver.h"
#include "ringBuffer.h"
#include "LEDDriver.h"
#include "timer0.h"




#define commandIssued		serialRetrieve
#define commandResponse(x)		serialPrint("\r\n[ RESPONSE ] : ");serialPrint(x);



#endif /* HEADERS_APPLICATION_COMMAND_H_ */
