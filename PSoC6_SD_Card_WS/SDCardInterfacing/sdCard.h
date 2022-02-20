/*
 * sdCard.h
 *
 *  Created on: 19-Feb-2022
 *      Author: denzo
 */

#ifndef SDCARD_H_
#define SDCARD_H_


#include "common.h"


typedef enum{
	CARD_ABSENT,
	CARD_DETECT_DEBOUNC_WAIT,
	CARD_DETECT_ACTIVITY,
	CARD_INSERT_CHECK,
	CARD_INITIALIZE,
	CARD_INSERTED_INITIALIZED,


}sdCardProcess_t;

void SD_Host_Init();
void SD_HostInit();
cy_en_sd_host_status_t cardInit();
void SD_printSDHC_1_cardType();
void SD_printSDHC_1_cardCapacity();
void cardDetectTimerInit();
void sdCardProcess();




#endif /* SDCARD_H_ */
