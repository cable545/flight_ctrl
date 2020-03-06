
#include "ext_flash.h"
#include "nvic.h"
#include "spi.h"

uint8_t EXT_FLASH_readDeviceId(void)
{
	uint8_t deviceId;

	CS_FLASH_ACTIVATE;

	SPI3_TransferByte(MANUFACTURER_DEVICE_ID_INSTR);
	SPI3_TransferByte(0);
	SPI3_TransferByte(0);
	SPI3_TransferByte(0);
	SPI3_TransferByte(0);
	deviceId = SPI3_TransferByte(0);

	CS_FLASH_DEACTIVATE;

	return deviceId;
}

uint8_t EXT_FLASH_readManufacturerId(void)
{
	uint8_t manufacturerId;

	CS_FLASH_ACTIVATE;

	SPI3_TransferByte(MANUFACTURER_DEVICE_ID_INSTR);
	SPI3_TransferByte(0);
	SPI3_TransferByte(0);
	SPI3_TransferByte(0);
	manufacturerId = SPI3_TransferByte(0);

	CS_FLASH_DEACTIVATE;

	return manufacturerId;
}

uint8_t EXT_FLASH_readByte(uint8_t* address)
{
	uint8_t value = 0;

	CS_FLASH_ACTIVATE;

	SPI3_TransferByte(READ_DATA_INSTR);
	SPI3_TransferByte(address[0]);
	SPI3_TransferByte(address[1]);
	SPI3_TransferByte(address[2]);
	value = SPI3_TransferByte(0);

	CS_FLASH_DEACTIVATE;

	return value;
}

void EXT_FLASH_readBytes(uint8_t* address, uint8_t* values, uint8_t length)
{
	disable();

	CS_FLASH_ACTIVATE;

	SPI3_TransferByte(READ_DATA_INSTR);
	SPI3_TransferByte(address[0]);
	SPI3_TransferByte(address[1]);
	SPI3_TransferByte(address[2]);

	for(uint32_t i = 0; i < length; i++)
		values[i] = SPI3_TransferByte(0);

	CS_FLASH_DEACTIVATE;

	enable();
}

void EXT_FLASH_writeByte(uint8_t* address, uint8_t value)
{
	EXT_FLASH_writeEnable();

	CS_FLASH_ACTIVATE;

	SPI3_TransferByte(PAGE_PROGRAM_INSTR);
	SPI3_TransferByte(address[0]);
	SPI3_TransferByte(address[1]);
	SPI3_TransferByte(address[2]);
	SPI3_TransferByte(value);

	CS_FLASH_DEACTIVATE;

	EXT_FLASH_writeDisable();
}

void EXT_FLASH_writeBytes(uint8_t* address, uint8_t* values, uint8_t length)
{
	disable();

	EXT_FLASH_writeEnable();
	CS_FLASH_ACTIVATE;

	SPI3_TransferByte(PAGE_PROGRAM_INSTR);
	SPI3_TransferByte(address[0]);
	SPI3_TransferByte(address[1]);
	SPI3_TransferByte(address[2]);
	for(uint32_t i = 0; i < length; i++)
		SPI3_TransferByte(values[i]);

	CS_FLASH_DEACTIVATE;
	EXT_FLASH_writeDisable();

	enable();
}

void EXT_FLASH_writeEnable(void)
{
	CS_FLASH_ACTIVATE;
	SPI3_TransferByte(WRITE_ENABLE_INSTR);
	CS_FLASH_DEACTIVATE;
}

void EXT_FLASH_writeDisable(void)
{
	CS_FLASH_ACTIVATE;
	SPI3_TransferByte(WRITE_DISABLE_INSTR);
	CS_FLASH_DEACTIVATE;
}

void EXT_FLASH_eraseChip(void)
{
	EXT_FLASH_writeEnable();
	CS_FLASH_ACTIVATE;
	SPI3_TransferByte(CHIP_ERASE_INSTR);
	CS_FLASH_DEACTIVATE;
	EXT_FLASH_writeDisable();
}

void EXT_FLASH_eraseSector(uint32_t sectorNumber)
{
	uint32_t address = sectorNumber * SECTOR_SIZE;

	EXT_FLASH_writeEnable();
	CS_FLASH_ACTIVATE;
	SPI3_TransferByte(SECTOR_ERASE_INSTR);
	CS_FLASH_DEACTIVATE;
	EXT_FLASH_writeDisable();
}

void EXT_FLASH_eraseBlock(uint32_t blockNumber)
{

}

int EXT_FLASH_readStatusRegister(uint8_t statusRegisterNumber)
{
	uint8_t instructionCode = 0x00, value = 0x00;

	switch(statusRegisterNumber)
	{
		case FIRST_STATUS_REGISTER:
			instructionCode = READ_FIRST_STATUS_REG_INSTR;
			break;
		case SECOND_STATUS_REGISTER:
			instructionCode = READ_SECOND_STATUS_REG_INSTR;
			break;
		case THIRD_STATUS_REGISTER:
			instructionCode = READ_THIRD_STATUS_REG_INSTR;
			break;
		default:
			return -1;
	}

	CS_FLASH_ACTIVATE;
	SPI3_TransferByte(instructionCode);
	value = SPI3_TransferByte(0);
	CS_FLASH_DEACTIVATE;

	return value;
}

void EXT_FLASH_writeStatusRegister(uint8_t statusRegisterNumber, uint8_t value)
{
	uint8_t instructionCode = 0x00;

	switch(statusRegisterNumber)
	{
		case FIRST_STATUS_REGISTER:
			instructionCode = WRITE_FIRST_STATUS_REG_INSTR;
			break;
		case SECOND_STATUS_REGISTER:
			instructionCode = WRITE_SECOND_STATUS_REG_INSTR;
			break;
		case THIRD_STATUS_REGISTER:
			instructionCode = WRITE_THIRD_STATUS_REG_INSTR;
			break;
		default:
			instructionCode = WRITE_FIRST_STATUS_REG_INSTR;
			break;
	}

	EXT_FLASH_writeEnable();
	CS_FLASH_ACTIVATE;
	SPI3_TransferByte(instructionCode);
	SPI3_TransferByte(value);
	CS_FLASH_DEACTIVATE;
	EXT_FLASH_writeDisable();
}
