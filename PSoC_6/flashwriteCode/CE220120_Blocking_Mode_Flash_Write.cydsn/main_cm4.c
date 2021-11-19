/*******************************************************************************
* File Name: main_cm4.c
*
* Version: 1.20
*
* Description: Demonstrates writing into the flash using a Peripheral Driver 
*              Library (PDL) API. The flash write blocks the caller until
*              the flash write is complete.
*
* Related Document: CE220120.pdf
*
* Hardware Dependency: CY8CKIT-062-BLE PSoC 6 BLE Pioneer Kit
*
********************************************************************************
* Copyright (2017-2020), Cypress Semiconductor Corporation.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*******************************************************************************/

#include "project.h"
#include <string.h>
#include <stdio.h>


/* Set this macro to '1' to observe the failure case of flash write. A RAM
 * address is passed as row address argument to the flash write function instead
 * of a flash address causing the function to return error.
 */
#define MAKE_FLASH_WRITE_FAIL       (0u)

/* This array reserves space in the flash for one row of size 
 * CY_FLASH_SIZEOF_ROW. Explicit initialization is required so that memory is
 * allocated in flash instead of RAM. CY_ALIGN ensures that the array address
 * is an integer multiple of the row size so that the array occupies one
 * complete row. 
 */
CY_ALIGN(CY_FLASH_SIZEOF_ROW) 
#if(MAKE_FLASH_WRITE_FAIL == 0)
    
    //const uint8_t flashData->myFlashData[CY_FLASH_SIZEOF_ROW] = {0}; /* The array will be placed in Flash */
   // const uint8_t* flashData->myFlashData = CY_FLASH_BASE;
    
    
#define LAST_FLASH_ROW 2047u
    
// 0x1000000 + 2047* 128*4 = 0x100FFE00
#define CALCULATE_FLASH_ADDRESS(rowNum) (CY_FLASH_BASE + ((rowNum) * CY_FLASH_SIZEOF_ROW))
CY_ALIGN(CY_FLASH_SIZEOF_ROW)

typedef struct{
    uint8_t myFlashData[100];
}flashData_s;

const flashData_s *flashData= (int16_t *)CALCULATE_FLASH_ADDRESS(LAST_FLASH_ROW);

#else
    const uint8_t *flashData->myFlashData = (uint8_t *)CY_SRAM0_BASE; /* Make the address point to some RAM location */
#endif  /* #if(MAKE_FLASH_WRITE_FAIL == 0) */
    
/* Array to hold the data to be written into flash */
uint8_t ramData[CY_FLASH_SIZEOF_ROW];


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary: This is the main entry for Cortex-M4. This function does the 
* following. 
* 1. Read and print the data from flash that was stored in previous power on [ DIF ]
* 2. Initializes the data into RAM that will be later written into flash and print it in UART [ DLF ]
* 3. Writes the data into flash
* 4. Verifies the data written into flash by comparing it with the RAM data
* 5. Turns the Green LED on if the flash write is successful otherwise turns the
*    Red LED on. 
* 
* The printf messages can be observed on P5_0 and P5_1 lines via COM using Teraterm or Bridge control panel UART.
* Retarget IO is enabled.
*
* Parameters:
*  None
*
* Return:
*  int
*
* Side Effects:
*  None  
*
*******************************************************************************/

int main(void)
{
    uint32_t index; 
    cy_en_flashdrv_status_t flashWriteStatus;
    
    //Default led state as red
    Cy_GPIO_Clr(LED_ERROR_PORT, LED_ERROR_NUM);
    Cy_GPIO_Set(LED_OK_PORT, LED_OK_NUM);
    
    UART_1_Start();
    
    __enable_irq(); /* Enable global interrupts. */
    printf("\x1b[2J\x1b[;H");
    
    printf("\r\nAccessing flash location : %X",flashData);
    
    printf("\r\n\r\n[ DIF ] : Reading Flash : ");
    for(index = 0; index < 10; index++)
    {
        /*
        * Simple read of the data from flash is done by pointer access
        * separate read function is not necessary to read the stored data from flash
        * Because the address of flashData->myFlashData array is known to us and it is hardcoded.
        */
        printf("%X ", (uint8_t)flashData->myFlashData[index]); 
    }

    
    printf("\r\n[ DLF ] : Writing Flash : ");
    /* Initialize the data in RAM that will be written into flash */
    for(index = 0; index < 10; index++)
    {
        ramData[index] = (uint8_t)index; 
        printf("%X ", ramData[index]); 
    }
    printf("\r\n\r\n\r\nIf you have just programmed the device, then [ DIF ] will be all 0's");
    printf("\r\nPerform a power cycle by clicking on the reset button");
    printf("\r\nAfter the reset, you will see that [ DIF ] will be the values which was in [ DLF ].");
    printf("\r\nThis means that the data that was stored in flash was successfully retrieved after power cycle.\r\n");
    
    /* Cy_Flash_WriteRow() is a blocking API that does not return until the
     * flash write is complete. The format of the rowAddr argument should be a 
     * 32-bit system address and any address within the row is a valid value.  
     */
    flashWriteStatus = Cy_Flash_WriteRow((uint32_t)flashData->myFlashData, (const uint32_t *)ramData);

    if((flashWriteStatus == CY_FLASH_DRV_SUCCESS) &&
       (memcmp(ramData, flashData->myFlashData, CY_FLASH_SIZEOF_ROW) == 0u))
    {
        /* Turn the Ok/Green LED on */
        Cy_GPIO_Clr(LED_OK_PORT, LED_OK_NUM);
    }
    else
    {
        /* Turn the Error/Red LED on */
        Cy_GPIO_Clr(LED_ERROR_PORT, LED_ERROR_NUM);
    }
    
    
    
    
    for(;;)
    {
        /* Put CPU into deep sleep mode to save power. This puts only CM4 into
         * deep sleep. Device enters deep sleep only when both the cores enter
         * deep sleep.
         */
        Cy_SysPm_DeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
    }
}

/* [] END OF FILE */
