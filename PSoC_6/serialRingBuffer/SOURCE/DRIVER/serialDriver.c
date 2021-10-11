/*
 * serialDriver.c
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */



#include "serialDriver.h"
#include "ringBuffer.h"
#include "uart.h"

bool bUartTxCmpltFlag = true;
ringStruct_s serialRing;
reception_s	receivedData;
ringQueue_s ringQueueHandle;

void ReceptionCallback(uint32_t event)
{

	switch(event)
	{
	case CY_SCB_UART_RECEIVE_NOT_EMTPY:

			ringStore_Byte(&serialRing,(uint8_t)uart_5_receive());
		break;
		case CY_SCB_UART_RECEIVE_DONE_EVENT :
			cyhal_gpio_write(CYBSP_USER_LED2,1);
		break;
		case CY_SCB_UART_TRANSMIT_DONE_EVENT:

			//Toggle the LED when a transmission complete even occurs
			cyhal_gpio_toggle(CYBSP_USER_LED1);

			//Set the transmission complete flag
			bUartTxCmpltFlag = true;
			break;
	}

}

void serialDebugInit()
{
	uart_5_Init(ReceptionCallback);

	ringBufferInit(&serialRing);

	ringQueueInit(&ringQueueHandle);

}


void SerialDebug_TASK()
{

	if(bUartTxCmpltFlag == true && ringQueueHandle.ringQueueEmptyFlag == false)
	{
		char tempBuffer[50];
		QueueRetrieve_ByteArray(&ringQueueHandle,tempBuffer);
		serialDebugTransmit(tempBuffer);
	}


	if(serialRing.ringBufferEmptyFlag == false)
	{
#ifdef _WORKING_SINGLE_SHOT
		receivedData.length = ringRetrieve_ByteArray(&serialRing,&receivedData.previousByte,10);
		for(int index = 0;index < receivedData.length; index++)
		{
			receivedData.receptionData[receivedData.index++] = receivedData.previousByte[index];
			if(receivedData.previousByte[index] == '\r' || receivedData.previousByte[index] == '\n' || receivedData.index >= (RECEPTION_BUFFER_MAX - 2))
			{
				receivedData.receptionData[receivedData.index++] = (char)'\r';
				receivedData.receptionData[receivedData.index++] = (char)'\n';
				receivedData.receptionData[receivedData.index] = (char)'\0';
				serialPrint((char*)&receivedData.receptionData);
				receivedData.index = 0;
				receivedData.receptionData[receivedData.index] = (char)'\0';
			}
		}
#endif

#ifdef _WORKING_UART_RECEPTION
		ringRetrieveByte(&serialRing,&receivedData.previousByte);
		receivedData.receptionData[receivedData.index++] = *receivedData.previousByte;
		if(*receivedData.previousByte == '\r' || *receivedData.previousByte == '\n' || receivedData.index >= (RECEPTION_BUFFER_MAX - 2))
		{
			receivedData.receptionData[receivedData.index++] = (char)'\r';
			receivedData.receptionData[receivedData.index++] = (char)'\n';
			receivedData.receptionData[receivedData.index] = (char)'\0';
			serialPrint((char*)&receivedData.receptionData);
			receivedData.index = 0;
			receivedData.receptionData[receivedData.index] = (char)'\0';
		}
#endif
	}
}


void serialDebugTransmit(char* transmitData)
{
	if(bUartTxCmpltFlag == false)
	{
		ringQueueStore(&ringQueueHandle,transmitData);
		return;
	}

	/* Start transmit operation (do not check status) */
	uart_5_Transmit(transmitData);

	/*set the falg to false after transmission*/
	bUartTxCmpltFlag = false;
}
