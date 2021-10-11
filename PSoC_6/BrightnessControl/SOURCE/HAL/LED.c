/*
 * LED.c
 *
 *  Created on: 09-Oct-2021
 *      Author: AustinA
 */

#include "LED.h"

#ifndef _RGB_GPIO_SCRATCH_MODE_DISABLE

const cy_stc_gpio_pin_config_t CYBSP_LED_RGB_RED_config =
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_LED_RGB_RED_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_LED_RGB_RED_obj =
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_LED_RGB_RED_PORT_NUM,
		.channel_num = CYBSP_LED_RGB_RED_PIN,
	};
#endif //defined (CY_USING_HAL)
const cy_stc_gpio_pin_config_t CYBSP_LED_RGB_BLUE_config =
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_LED_RGB_BLUE_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_LED_RGB_BLUE_obj =
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_LED_RGB_BLUE_PORT_NUM,
		.channel_num = CYBSP_LED_RGB_BLUE_PIN,
	};
#endif //defined (CY_USING_HAL)

const cy_stc_gpio_pin_config_t CYBSP_LED_RGB_GREEN_config =
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_LED_RGB_GREEN_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_LED_RGB_GREEN_obj =
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_LED_RGB_GREEN_PORT_NUM,
		.channel_num = CYBSP_LED_RGB_GREEN_PIN,
	};
#endif //defined (CY_USING_HAL)
#endif


#ifndef _LED_8_9_GPIO_SCRATCH_MODE_DISABLE

const cy_stc_gpio_pin_config_t CYBSP_LED9_config =
{
	.outVal = 1,
	.driveMode = CY_GPIO_DM_STRONG,
	.hsiom = CYBSP_LED9_HSIOM,
	.intEdge = CY_GPIO_INTR_DISABLE,
	.intMask = 0UL,
	.vtrip = CY_GPIO_VTRIP_CMOS,
	.slewRate = CY_GPIO_SLEW_FAST,
	.driveSel = CY_GPIO_DRIVE_1_2,
	.vregEn = 0UL,
	.ibufMode = 0UL,
	.vtripSel = 0UL,
	.vrefSel = 0UL,
	.vohSel = 0UL,
};
#if defined (CY_USING_HAL)
	const cyhal_resource_inst_t CYBSP_LED9_obj =
	{
		.type = CYHAL_RSC_GPIO,
		.block_num = CYBSP_LED9_PORT_NUM,
		.channel_num = CYBSP_LED9_PIN,
	};
#endif //defined (CY_USING_HAL)

	const cy_stc_gpio_pin_config_t CYBSP_LED8_config =
	{
		.outVal = 1,
		.driveMode = CY_GPIO_DM_STRONG,
		.hsiom = CYBSP_LED8_HSIOM,
		.intEdge = CY_GPIO_INTR_DISABLE,
		.intMask = 0UL,
		.vtrip = CY_GPIO_VTRIP_CMOS,
		.slewRate = CY_GPIO_SLEW_FAST,
		.driveSel = CY_GPIO_DRIVE_1_2,
		.vregEn = 0UL,
		.ibufMode = 0UL,
		.vtripSel = 0UL,
		.vrefSel = 0UL,
		.vohSel = 0UL,
	};
	#if defined (CY_USING_HAL)
		const cyhal_resource_inst_t CYBSP_LED8_obj =
		{
			.type = CYHAL_RSC_GPIO,
			.block_num = CYBSP_LED8_PORT_NUM,
			.channel_num = CYBSP_LED8_PIN,
		};
	#endif //defined (CY_USING_HAL)
#endif


void LEDsInit()
{
#ifndef _RGB_GPIO_SCRATCH_MODE_DISABLE

		Cy_GPIO_Pin_Init(CYBSP_LED_RGB_RED_PORT, CYBSP_LED_RGB_RED_PIN, &CYBSP_LED_RGB_RED_config);
	#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_LED_RGB_RED_obj);
	#endif //defined (CY_USING_HAL)

		Cy_GPIO_Pin_Init(CYBSP_LED_RGB_BLUE_PORT, CYBSP_LED_RGB_BLUE_PIN, &CYBSP_LED_RGB_BLUE_config);
	#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_LED_RGB_BLUE_obj);

	#endif //defined (CY_USING_HAL)

		Cy_GPIO_Pin_Init(CYBSP_LED_RGB_GREEN_PORT, CYBSP_LED_RGB_GREEN_PIN, &CYBSP_LED_RGB_GREEN_config);
	#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_LED_RGB_GREEN_obj);
	#endif //defined (CY_USING_HAL)

#endif

#ifndef _LED_8_9_GPIO_SCRATCH_MODE_DISABLE

		Cy_GPIO_Pin_Init(CYBSP_LED9_PORT, CYBSP_LED9_PIN, &CYBSP_LED9_config);
	#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_LED9_obj);
	#endif //defined (CY_USING_HAL)


		Cy_GPIO_Pin_Init(CYBSP_LED8_PORT, CYBSP_LED8_PIN, &CYBSP_LED8_config);
	#if defined (CY_USING_HAL)
		cyhal_hwmgr_reserve(&CYBSP_LED8_obj);
	#endif //defined (CY_USING_HAL)

#endif

}


