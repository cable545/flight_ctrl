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

#define CS_IMU_ACTIVATE	(GPIO_LOW(GPIOD, GPIO_PIN_2))
#define CS_IMU_DEACTIVATE	(GPIO_HIGH(GPIOD, GPIO_PIN_2))

// Acc and Gyro register addresses

#define MPU9250_SMPLRT_DIV					0x19
#define MPU9250_GYRO_LPF            0x1a
#define MPU9250_GYRO_CONFIG         0x1b
#define MPU9250_ACCEL_CONFIG        0x1c
#define MPU9250_ACCEL_LPF           0x1d
#define MPU9250_FIFO_EN             0x23
#define MPU9250_I2C_MST_CTRL        0x24
#define MPU9250_I2C_SLV0_ADDR       0x25
#define MPU9250_I2C_SLV0_REG        0x26
#define MPU9250_I2C_SLV0_CTRL       0x27
#define MPU9250_I2C_SLV1_ADDR       0x28
#define MPU9250_I2C_SLV1_REG        0x29
#define MPU9250_I2C_SLV1_CTRL       0x2a
#define MPU9250_I2C_SLV2_ADDR       0x2b
#define MPU9250_I2C_SLV2_REG        0x2c
#define MPU9250_I2C_SLV2_CTRL       0x2d
#define MPU9250_I2C_SLV4_CTRL       0x34
#define MPU9250_INT_PIN_CFG         0x37
#define MPU9250_INT_ENABLE          0x38
#define MPU9250_INT_STATUS          0x3a
#define MPU9250_ACCEL_XOUT_H        0x3b
#define MPU9250_GYRO_XOUT_H         0x43
#define MPU9250_EXT_SENS_DATA_00    0x49
#define MPU9250_I2C_SLV1_DO         0x64
#define MPU9250_I2C_MST_DELAY_CTRL  0x67
#define MPU9250_USER_CTRL           0x6a
#define MPU9250_PWR_MGMT_1          0x6b
#define MPU9250_PWR_MGMT_2          0x6c
#define MPU9250_FIFO_COUNT_H        0x72
#define MPU9250_FIFO_R_W            0x74
#define GYRO_ACC_WHO_AM_I_REG_ADDR	0x75

// Acc and Gyro register values

#define GYRO_ACC_WHO_AM_I_DEFAULT		0x71

//  Gyro LPF options

#define MPU9250_GYRO_LPF_8800       0x11                    // 8800Hz, 0.64mS delay
#define MPU9250_GYRO_LPF_3600       0x10                    // 3600Hz, 0.11mS delay
#define MPU9250_GYRO_LPF_250        0x00                    // 250Hz, 0.97mS delay
#define MPU9250_GYRO_LPF_184        0x01                    // 184Hz, 2.9mS delay
#define MPU9250_GYRO_LPF_92         0x02                    // 92Hz, 3.9mS delay
#define MPU9250_GYRO_LPF_41         0x03                    // 41Hz, 5.9mS delay
#define MPU9250_GYRO_LPF_20         0x04                    // 20Hz, 9.9mS delay
#define MPU9250_GYRO_LPF_10         0x05                    // 10Hz, 17.85mS delay
#define MPU9250_GYRO_LPF_5          0x06                    // 5Hz, 33.48mS delay

//  Gyro FSR options

#define MPU9250_GYROFSR_250         0                       // +/- 250 degrees per second
#define MPU9250_GYROFSR_500         8                       // +/- 500 degrees per second
#define MPU9250_GYROFSR_1000        0x10                    // +/- 1000 degrees per second
#define MPU9250_GYROFSR_2000        0x18                    // +/- 2000 degrees per second

//  Accel FSR options

#define MPU9250_ACCELFSR_2          0                       // +/- 2g
#define MPU9250_ACCELFSR_4          8                       // +/- 4g
#define MPU9250_ACCELFSR_8          0x10                    // +/- 8g
#define MPU9250_ACCELFSR_16         0x18                    // +/- 16g

// Acc LPF options

#define MPU9250_ACCEL_LPF_1046      0x08                    // 1046Hz, 0.50mS delay
#define MPU9250_ACCEL_LPF_218       0x01                    // 218.1Hz, 1.88mS delay
#define MPU9250_ACCEL_LPF_99        0x02                    // 99Hz, 2.88mS delay
#define MPU9250_ACCEL_LPF_45        0x03                    // 44.8Hz, 4.88mS delay
#define MPU9250_ACCEL_LPF_21        0x04                    // 21.2Hz, 8.87mS delay
#define MPU9250_ACCEL_LPF_10        0x05                    // 10.2Hz, 16.83mS delay
#define MPU9250_ACCEL_LPF_5         0x06                    // 5.05Hz, 32.48mS delay
#define MPU9250_ACCEL_LPF_420       0x07                    // 420Hz, 1.38mS delay

// Mag register addresses

#define AK8963_DEVICE_ID_REG_ADDR		0x00

// Mag register values

#define AK8963_DEVICE_ID						0x48

void MPU_Init(void);
uint8_t MPU_readWhoAmIReg(void);
uint8_t MPU_readWhoAmIReg2(void);
void MPU_ReadAccData(int16_t* buffer);
void MPU_ReadScaledAccData(float* buffer);
void MPU_ReadGyroData(int16_t* buffer);
void MPU_ReadScaledGyroData(float* buffer);

#endif
