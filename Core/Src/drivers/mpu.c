/*
 * imu.c
 *
 *  Created on: 20.01.2020
 *      Author: burde
 */

#include "drivers/mpu.h"
#include "drivers/spi.h"

uint8_t MPU_readWhoAmIReg(void)
{
	uint8_t whoAmIIndicator = 0;

	CS_IMU_ACTIVATE;

	SPI3_TransferByte(GYRO_ACC_WHO_AM_I_REG_ADDR | MPU_READ_BIT);
	whoAmIIndicator = SPI3_TransferByte(0);

	CS_IMU_DEACTIVATE;

	return whoAmIIndicator;
}

uint8_t MPU_readWhoAmIReg2 (void)
{
	uint8_t whoAmIIndicator = 0;

	CS_IMU_ACTIVATE;

	SPI3_TransferByte(MAG_DEVICE_ID_REG_ADDR | MPU_READ_BIT);
	whoAmIIndicator = SPI3_TransferByte(0);

	CS_IMU_DEACTIVATE;

	return whoAmIIndicator;
}
