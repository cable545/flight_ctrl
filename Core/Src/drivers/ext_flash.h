#ifndef __EXT_FLASH_H
#define __EXT_FLASH_H

#include "platform.h"

#include "io.h"

#define FIRST_STATUS_REGISTER					0x00
#define SECOND_STATUS_REGISTER				0x01
#define THIRD_STATUS_REGISTER					0x02


// instructions
#define WRITE_ENABLE_INSTR 						0x06
#define WRITE_DISABLE_INSTR 					0x04
#define ENABLE_RESET_INSTR 						0x66
#define RESET_INSTR										0x99
#define MANUFACTURER_DEVICE_ID_INSTR	0x90
#define READ_DATA_INSTR								0x03
#define FAST_READ_DATA_INSTR					0x0B
#define PAGE_PROGRAM_INSTR						0x02
#define CHIP_ERASE_INSTR							0x60
#define SECTOR_ERASE_INSTR						0x20
#define HALFBLOCK_ERASE_INSTR					0x52
#define BLOCK_ERASE_INSTR							0xD8
#define READ_FIRST_STATUS_REG_INSTR		0x05
#define READ_SECOND_STATUS_REG_INSTR	0x35
#define READ_THIRD_STATUS_REG_INSTR		0x15
#define WRITE_FIRST_STATUS_REG_INSTR	0x01
#define WRITE_SECOND_STATUS_REG_INSTR	0x31
#define WRITE_THIRD_STATUS_REG_INSTR	0x11

#define SECTOR_SIZE										0x001000
#define HALF_BLOCK_SIZE								0x008000
#define BLOCK_SIZE										0x010000
#define MAX_MEMORY_ADDRESS						0xFFFFFF

#define CS_FLASH_ACTIVATE	(GPIO_LOW(GPIOA,GPIO_PIN_1))
#define CS_FLASH_DEACTIVATE	(GPIO_HIGH(GPIOA,GPIO_PIN_1))

uint8_t EXT_FLASH_readDeviceId(void);
uint8_t EXT_FLASH_readManufacturerId(void);
uint8_t EXT_FLASH_readByte(uint8_t* address);
void EXT_FLASH_readBytes(uint8_t* address, uint8_t* values, uint8_t length);
void EXT_FLASH_writeByte(uint8_t* address, uint8_t value);
void EXT_FLASH_writeBytes(uint8_t* address, uint8_t* values, uint8_t length);
void EXT_FLASH_writeEnable(void);
void EXT_FLASH_writeDisable(void);
void EXT_FLASH_eraseChip(void);
void EXT_FLASH_eraseSector(uint32_t sectorNumber);
void EXT_FLASH_eraseBlock(uint32_t blockNumber);
int EXT_FLASH_readStatusRegister(uint8_t statusRegisterNumber);
void EXT_FLASH_writeStatusRegister(uint8_t statusRegisterNumber, uint8_t value);

#endif
