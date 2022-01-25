/*
 * motionSensor.c
 *
 *  Created on: 30-Nov-2021
 *      Author: AustinA
 */



#include "motionSensor.h"
#include "cy8ckit_028_epd_pins.h"
#include "eink_task.h"



mtb_bmi160_t motion_sensor;
cyhal_i2c_t i2c;
cyhal_i2c_cfg_t i2c_cfg = {
    .is_slave = false,
    .address = 0,
    .frequencyhal_hz = 400000
};


#define IMU_I2C_SDA (CY8CKIT_028_EPD_PIN_IMU_I2C_SDA) // Define me
#define IMU_I2C_SCL (CY8CKIT_028_EPD_PIN_IMU_I2C_SCL) // Define me

void motion_task()
{

    cy_rslt_t result;
    /* Initialize i2c for motion sensor */
    result = cyhal_i2c_init(&i2c, IMU_I2C_SDA, IMU_I2C_SCL, NULL);
    CY_ASSERT(result == CY_RSLT_SUCCESS);
    result = cyhal_i2c_configure(&i2c, &i2c_cfg);
    CY_ASSERT(result == CY_RSLT_SUCCESS);
    /* Initialize motion sensor */
    result = mtb_bmi160_init_i2c(&motion_sensor, &i2c, MTB_BMI160_DEFAULT_ADDRESS);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    printf("Motion Sensor Initialized \r\n");
    for (;;)
        {
            /* Get the accel and gyro data and print the results to the UART */
            mtb_bmi160_data_t data;
            mtb_bmi160_read(&motion_sensor, &data);

            if(geteinkTaskReadyFlag())
    		if(xQueueSend(motionDataQueue,&data, ( TickType_t ) 10 ) != pdPASS )
			{
				printf(" Failed to post the message, even after 10 ticks.\r\n ");
			}
//            printf("Accel: X:%6d Y:%6d Z:%6d\r\n", data.accel.x, data.accel.y, data.accel.z);
//            printf("Gyro : X:%6d Y:%6d Z:%6d\r\n\r\n", data.gyro.x, data.gyro.y, data.gyro.z);
//            vTaskDelay(10);
        }
}
