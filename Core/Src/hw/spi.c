#include "platform.h"

#include "hw/spi.h"

//#include "stm32f4xx.h"
//#include "stm32f4xx_spi.h"

#define ENABLE_SPI3() SPI3->CR1 |= SPI_CR1_SPE
#define DISABLE_SPI3() SPI3->CR1 &= ~SPI_CR1_SPE

#define CR1_CLEAR_MASK	((uint16_t)0x3040)

static void SPI3_LowLevelInit(void)
{
	uint16_t tmpreg = 0;
	/* Get the SPIx CR1 value */
	tmpreg = SPI3->CR1;
	/* Clear BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL and CPHA bits */
	tmpreg &= CR1_CLEAR_MASK;
	/* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
		 master/salve mode, CPOL and CPHA */
	/* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
	/* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values */
	/* Set LSBFirst bit according to SPI_FirstBit value */
	/* Set BR bits according to SPI_BaudRatePrescaler value */
	/* Set CPOL bit according to SPI_CPOL value */
	/* Set CPHA bit according to SPI_CPHA value */
	tmpreg |= (uint16_t)((uint32_t)	SPI_DIRECTION_2LINES | SPI_MODE_MASTER | SPI_DATASIZE_8BIT | SPI_POLARITY_HIGH |
																	SPI_PHASE_2EDGE | SPI_NSS_SOFT | SPI_BAUDRATEPRESCALER_32 | SPI_FIRSTBIT_MSB);
	/* Write to SPIx CR1 */
	SPI3->CR1 = tmpreg;

	/* Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register) */
	SPI3->I2SCFGR &= (uint16_t)~((uint16_t)SPI_I2SCFGR_I2SMOD);
/*---------------------------- SPIx CRCPOLY Configuration --------------------*/
	/* Write to SPIx CRCPOLY */
	SPI3->CRCPR = 7;
}

void SPI3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* Configure SPI pins: SCK, MOSI, MISO  */
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStructure.Alternate = GPIO_AF6_SPI3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	__HAL_RCC_SPI3_CLK_ENABLE();
	DISABLE_SPI3();
	SPI3_LowLevelInit();
	ENABLE_SPI3();
}

uint8_t SPI3_TransferByte(uint8_t tx)
{
	uint16_t rx;

	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	while((SPI3->SR & SPI_FLAG_TXE) == 0);
	/* Send SPI data */
	SPI3->DR = (uint16_t) tx;
	/* Wait for SPI1 data reception */
	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	while((SPI3->SR & SPI_FLAG_RXNE) == 0);
	/* Read SPI received data */
	rx = SPI3->DR;

	return ((uint8_t) rx);
}

uint16_t SPI3_TransferWord(uint16_t tx)
{
	uint16_t rx;

	while((SPI3->SR & SPI_FLAG_TXE) == 0);
	/* Send SPI data */
	SPI3->DR = (uint16_t) tx;
	/* Wait for SPI1 data reception */
	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	while((SPI3->SR & SPI_FLAG_RXNE) == 0);
	/* Read SPI received data */
	rx = SPI3->DR;

	return rx;
}

/**
  * @brief Tx a mem block through spi interface to selected device
  * @par device specifier, *tx pointer to Tx/Rx buffer, length in bytes
  * @retval length of written/read block in bytes
  * @par Required preconditions: none
  */
uint16_t SPI3_Transfer(uint8_t device, uint8_t *txrx, uint16_t length)
{
	uint16_t l;

	l = length;

	while(0 < length)
	{
		*txrx = SPI3_TransferByte(*txrx);
		length--;
		txrx++;
	}

	return l;
}


