/*******************************************************************************
* File Name: cycfg_usbdev.h
*
* Description:
* USB Device middleware configuration
* This file should not be modified. It was automatically generated by
* USB Configurator 2.40.0.6118
*
********************************************************************************
* \copyright
* Copyright 2022 Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#if !defined(H_CYCFG_USBDEV)
#define H_CYCFG_USBDEV
    
#if defined(__cplusplus)
extern "C" {
#endif

#include "cy_usb_dev_audio.h"
#include "cy_usb_dev_hid.h"
#include "cy_usb_dev_cdc.h"

#define CY_USB_CFG_TOOL_VERSION                  (240)

/* Supported USB Middleware version */
#define CY_USB_DEV_MW_VERSION_REQUIRED           (200)

#if !defined (CY_USB_DEV_MW_VERSION)
    #if (CY_USB_DEV_MW_VERSION_MAJOR == 2)
        #define CY_USB_DEV_MW_VERSION            (200)
    #elif (CY_USB_DEV_MW_VERSION_MAJOR == 1)
        #define CY_USB_DEV_MW_VERSION            (100)
    #else
        #define CY_USB_DEV_MW_VERSION            (100)
    #endif
#endif

/* Check the used Middleware version */
#if (CY_USB_DEV_MW_VERSION_REQUIRED > CY_USB_DEV_MW_VERSION)
    #error The USB Configurator requires a newer version of the USB Middleware. Update the USB Middleware in your project.
#endif

/* Number of USB Device */
#define usb_NUM_DEVICES           (1)

/* Class specific defines */
#define usb_AUDIO_CLASS_SUPPORTED (0U)
#define usb_CDC_CLASS_SUPPORTED   (0U)
#define usb_HID_CLASS_SUPPORTED   (1U)

/* Array of USB Devices */
extern const cy_stc_usb_dev_device_t usb_devices[usb_NUM_DEVICES];

/* Device configuration */
extern const cy_stc_usb_dev_config_t usb_devConfig;
/* HID configuration */
extern const cy_stc_usb_dev_hid_config_t usb_hidConfig;

#if defined(__cplusplus)
}
#endif

#endif /* H_CYCFG_USBDEV */

/* [] END OF FILE */