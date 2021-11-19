/*
 * debug_task.c
 *
 *  Created on: 19-Nov-2021
 *      Author: austina
 */

#include "debug_task.h"


uint8_t retrievalBuffer[200];


void debug_ReceptionData_Handler(uint8_t* receptionData)
{
	serialPrint("Processing Command : ");
	serialPrint(receptionData);
	serialPrint("\r\n");
}



void debug_task()
{


    CyDelay(1);

    while(1)
    {
    	SerialDebug_TASK();

    	if(serialDebug_RetrieveReceptionData(retrievalBuffer))
    	{
    		debug_ReceptionData_Handler(retrievalBuffer);
    	}
    }


}

