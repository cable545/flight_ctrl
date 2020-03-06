/*
 * imu.h
 *
 *  Created on: 20.01.2020
 *      Author: burde
 */

#include "platform.h"

#include "drivers/io.h"

#ifndef __IMU_H
#define __IMU_H

#define MPU_READ_BIT								0x80
#define MPU_WRITE_BIT								0x00

#define GYRO_ACC_WHO_AM_I_REG_ADDR	0x75

#define MAG_DEVICE_ID_REG_ADDR			0x00

#define CS_IMU_ACTIVATE	(GPIO_LOW(GPIOD, GPIO_PIN_2))
#define CS_IMU_DEACTIVATE	(GPIO_HIGH(GPIOD, GPIO_PIN_2))

uint8_t MPU_readWhoAmIReg(void);
uint8_t MPU_readWhoAmIReg2(void);

#endif
