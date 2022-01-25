/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/
#include "cy_pdl.h"
#include "cy_usb_dev.h"
#include "cy_usb_dev_audio.h"
#include "cy_usb_dev_cdc.h"
#include "cy_usb_dev_hid.h"
#include "cycfg_usbdev.h"
#include "cycfg.h"
#include "cyhal.h"
#include "cybsp.h"

#define USBHID_HW	USBFS0
#define USBHID_config	usb_hidConfig

/*******************************************************************************
* Global constants
*******************************************************************************/
cy_stc_usbfs_dev_drv_context_t      USBHID_context;
cy_stc_usb_dev_context_t            USBHID_devContext;
cy_stc_usb_dev_hid_context_t        USBHID_hidContext;

/*******************************************************************************
* USBFS interrupts functions and configurations
*******************************************************************************/
static void USBD_IsrLow(void);
static void USBD_IsrMedium(void);
static void USBD_IsrHigh(void);
const cy_stc_sysint_t USBD_IntrHighConfig =
{
    .intrSrc = (IRQn_Type) usb_interrupt_hi_IRQn,
    .intrPriority = 5U,
};
const cy_stc_sysint_t USBD_IntrMeduimConfig =
{
    .intrSrc = (IRQn_Type) usb_interrupt_med_IRQn,
    .intrPriority = 6U,
};
const cy_stc_sysint_t USBD_IntrLowConfig =
{
    .intrSrc = (IRQn_Type) usb_interrupt_lo_IRQn,
    .intrPriority = 7U,
};

/*******************************************************************************
* Function Name: USBD_IsrHigh
*******************************************************************************/
static void USBD_IsrHigh(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USBHID_HW, Cy_USBFS_Dev_Drv_GetInterruptCauseHi(USBHID_HW),
                               &USBHID_context);
}
/*******************************************************************************
* Function Name: USBD_IsrMedium
*******************************************************************************/
static void USBD_IsrMedium(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USBHID_HW, Cy_USBFS_Dev_Drv_GetInterruptCauseMed(USBHID_HW),
                               &USBHID_context);
}
/*******************************************************************************
* Function Name: USBD_IsrLow
*******************************************************************************/
static void USBD_IsrLow(void)
{
    /* Call interrupt processing */
    Cy_USBFS_Dev_Drv_Interrupt(USBHID_HW, Cy_USBFS_Dev_Drv_GetInterruptCauseLo(USBHID_HW),
                               &USBHID_context);
}

int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    cy_en_usb_dev_status_t status;

	/* Initialize device hardware */
	//init_cycfg_all();
	/* Initialize USB */
	status = Cy_USB_Dev_Init(USBHID_HW, &usb_0_config, &USBHID_context,
							 &usb_devices[0], &usb_devConfig, &USBHID_devContext);
	if (CY_USB_DEV_SUCCESS != status)
	{
		/* Initialization error - stop execution */
		while(1);
	}
	status = Cy_USB_Dev_HID_Init(&usb_hidConfig, &USBHID_hidContext, &USBHID_devContext);
	if (CY_USB_DEV_SUCCESS != status)
	{
		/* HID Initialization error - stop execution */
		while(1);
	}
	/* Hook interrupt service routines */
	(void) Cy_SysInt_Init(&USBD_IntrLowConfig,    &USBD_IsrLow);
	(void) Cy_SysInt_Init(&USBD_IntrMeduimConfig, &USBD_IsrMedium);
	(void) Cy_SysInt_Init(&USBD_IntrHighConfig,   &USBD_IsrHigh);
	/* Enable interrupts */
	NVIC_EnableIRQ((IRQn_Type) USBD_IntrLowConfig.intrSrc);
	NVIC_EnableIRQ((IRQn_Type) USBD_IntrMeduimConfig.intrSrc);
	NVIC_EnableIRQ((IRQn_Type) USBD_IntrHighConfig.intrSrc);

	/* Enable interrupts */
	__enable_irq();

	/* Make device appear on the bus */
	Cy_USB_Dev_Connect(true, CY_USB_DEV_WAIT_FOREVER, &USBHID_devContext);



	const uint32_t CURSOR_X_POS     =   1UL;
	const uint32_t MOUSE_DATA_LEN   =   3UL;
	const uint32_t MOUSE_IN_EP      =   1UL;
	const uint8_t  CURSOR_STEP      =   5U;
	const uint32_t STEPS_NUMBER     =   96UL;
	uint8_t counter = 0U;
	bool moveRight = true;
	/* Mouse packet array: buttons (1st byte), X (2nd byte), Y (3rd byte) */
	CY_USB_DEV_ALLOC_ENDPOINT_BUFFER(mouseData, 3U);
	mouseData[2U] = 0U; /* No changes in Y - position */


    for(;;)
    {
        /* Move mouse to the right or to the left appropriate number of steps */
        mouseData[CURSOR_X_POS] = moveRight ? CURSOR_STEP : (uint8_t)-CURSOR_STEP;
        /* Define direction of the movement */
        if (0U == counter)
        {
            counter = STEPS_NUMBER;
            moveRight = !moveRight;
        }
        /* Update mouse position */
        Cy_USB_Dev_WriteEpBlocking(MOUSE_IN_EP, mouseData, MOUSE_DATA_LEN,
                                    CY_USB_DEV_WAIT_FOREVER, &USBHID_devContext);
        counter--;
        Cy_SysLib_Delay(1000UL);
    }
}

/* [] END OF FILE */
