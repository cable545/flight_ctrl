/*
 * imu.c
 *
 *  Created on: 20.01.2020
 *      Author: burde
 */

#include <stdbool.h>

#include "common/maths.h"

#include "drivers/mpu.h"

#include "hw/spi.h"

typedef struct
{
	uint32_t gyroAccSampleRate;
	uint32_t gyroAccSampleInterval;
	uint32_t magSampleRate;
	int32_t gyroLpf;
	int32_t accLpf;
	int32_t gyroFsr;
	int32_t accFsr;
	float gyroScale;
	float accScale;
} GyroAccSettings_t;

static GyroAccSettings_t gyroAccSettings;

static void readBytes(uint8_t address, uint8_t* bytes, uint32_t length)
{
	CS_IMU_ACTIVATE;

	SPI3_TransferByte(address | MPU_READ_BIT);

	for(uint32_t i = 0; i < length; i++)
		bytes[i] = SPI3_TransferByte(0);

	CS_IMU_DEACTIVATE;
}

static uint8_t readByte(uint8_t address)
{
	uint8_t byte = 0;

	readBytes(address, &byte, 1);

	return byte;
}

static void writeBytes(uint8_t address, uint8_t* bytes, uint32_t length)
{
	CS_IMU_ACTIVATE;

	SPI3_TransferByte(address);

	for(uint32_t i = 0; i < length; i++)
		SPI3_TransferByte(bytes[i]);

	CS_IMU_DEACTIVATE;
}

static void writeByte(uint8_t address, uint8_t byte)
{
	writeBytes(address, &byte, 1);
}

static void lowLevelInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOD_CLK_ENABLE();
	/* Configure CS pin for imu  */
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static bool setGyroScale(uint8_t fsr)
{
	bool result = false;

	switch(fsr)
	{
		case MPU9250_GYROFSR_250:
			gyroAccSettings.gyroScale = PI_f / (131.0f * 180.0f);
			result = true;

			break;
		case MPU9250_GYROFSR_500:
			gyroAccSettings.gyroScale = PI_f / (62.5f * 180.0f);
			result = true;

			break;
		case MPU9250_GYROFSR_1000:
			gyroAccSettings.gyroScale = PI_f / (32.8f * 180.0f);
			result = true;

			break;
		case MPU9250_GYROFSR_2000:
			gyroAccSettings.gyroScale = PI_f / (16.4f * 180.0f);
			result = true;

			break;
		default:
			break;
	}

	return result;
}

static bool setAccScale(uint8_t fsr)
{
	bool result = false;

	switch(fsr)
	{
		case MPU9250_ACCELFSR_2:
			gyroAccSettings.accScale = 1.0f / 16384.0f;
			result =  true;

			break;
		case MPU9250_ACCELFSR_4:
			gyroAccSettings.accScale = 1.0f / 8192.0f;
			result =  true;

			break;
		case MPU9250_ACCELFSR_8:
			gyroAccSettings.accScale = 1.0f / 4096.0f;
			result =  true;

			break;
		case MPU9250_ACCELFSR_16:
			gyroAccSettings.accScale = 1.0f / 2048.0f;
			result =  true;

			break;
		default:
			break;
	}

	return result;
}

static void setGyroConfig(void)
{
	uint8_t gyroConfig = gyroAccSettings.gyroFsr + ((gyroAccSettings.gyroLpf >> 3) & 3); // check output, not sure if the shift is correct
  uint8_t gyroLpf = gyroAccSettings.gyroLpf & 7;

  writeByte(MPU9250_GYRO_CONFIG, gyroConfig);
  writeByte(MPU9250_GYRO_LPF, gyroLpf);
}

static void setAccConfig(void)
{
	writeByte(MPU9250_ACCEL_CONFIG, gyroAccSettings.accFsr);
	writeByte(MPU9250_ACCEL_LPF, gyroAccSettings.accLpf);
}

static void setGyroAccSampleRate(void)
{

}

void MPU_Init(void)
{
	lowLevelInit();

	gyroAccSettings.gyroAccSampleRate = 80;
	gyroAccSettings.magSampleRate = 40;
	gyroAccSettings.gyroLpf = MPU9250_GYRO_LPF_41;
	gyroAccSettings.accLpf = MPU9250_ACCEL_LPF_45;
	gyroAccSettings.gyroFsr = MPU9250_GYROFSR_1000;
	gyroAccSettings.accFsr = MPU9250_ACCELFSR_8;

	setGyroScale(gyroAccSettings.gyroFsr);
	setAccScale(gyroAccSettings.accFsr);

	setGyroConfig();
	setAccConfig();
	setGyroAccSampleRate();
}

uint8_t MPU_readWhoAmIReg(void)
{
	return readByte(GYRO_ACC_WHO_AM_I_REG_ADDR);
}

uint8_t MPU_readWhoAmIReg2(void)
{
	return readByte(AK8963_DEVICE_ID_REG_ADDR);
}

void MPU_ReadAccData(int16_t* buffer)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  readBytes(MPU9250_ACCEL_XOUT_H, rawData, 6);  // Read the six raw data registers into data array
  buffer[0] = ((int16_t)rawData[0] << 8) | rawData[1];  // Turn the MSB and LSB into a signed 16-bit value
  buffer[1] = ((int16_t)rawData[2] << 8) | rawData[3];
  buffer[2] = ((int16_t)rawData[4] << 8) | rawData[5];
}

void MPU_ReadScaledAccData(float* buffer)
{
	int16_t tmpBuffer[3];
	MPU_ReadAccData(tmpBuffer);
	buffer[0] = (float)(tmpBuffer[0] * gyroAccSettings.accScale);
	buffer[1] = (float)(tmpBuffer[1] * gyroAccSettings.accScale);
	buffer[2] = (float)(tmpBuffer[2] * gyroAccSettings.accScale);
}

void MPU_ReadGyroData(int16_t* buffer)
{
  uint8_t rawData[6];
  readBytes(MPU9250_GYRO_XOUT_H, rawData, 6);
  buffer[0] = ((int16_t)rawData[0] << 8) | rawData[1];
  buffer[1] = ((int16_t)rawData[2] << 8) | rawData[3];
  buffer[2] = ((int16_t)rawData[4] << 8) | rawData[5];
}

void MPU_ReadScaledGyroData(float* buffer)
{
	int16_t tmpBuffer[3];
	MPU_ReadGyroData(tmpBuffer);
	buffer[0] = (float)(tmpBuffer[0] * gyroAccSettings.gyroScale);
	buffer[1] = (float)(tmpBuffer[1] * gyroAccSettings.gyroScale);
	buffer[2] = (float)(tmpBuffer[2] * gyroAccSettings.gyroScale);
}
