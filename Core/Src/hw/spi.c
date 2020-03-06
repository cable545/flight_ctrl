#include "platform.h"
#include "spi.h"

//#include "stm32f4xx.h"
//#include "stm32f4xx_spi.h"

#define ENABLE_SPI3() SPI3->CR1 |= SPI_CR1_SPE
#define DISABLE_SPI3() SPI3->CR1 &= ~SPI_CR1_SPE

void SPI3_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
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

	/* SPI3 Config -----------------------------------------------------*/
	SPI_InitStructure.Mode = SPI_MODE_MASTER;
	SPI_InitStructure.Direction = SPI_DIRECTION_2LINES;

	SPI_InitStructure.DataSize = SPI_DATASIZE_8BIT;
	SPI_InitStructure.CLKPolarity = SPI_POLARITY_LOW; // Low;
	SPI_InitStructure.CLKPhase = SPI_PHASE_1EDGE; // if driver version 4.0 and greater SPI_CPHA_1Edge else SPI_CPHA_2Edge;
	SPI_InitStructure.NSS = SPI_NSS_SOFT;
	SPI_InitStructure.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; // 42 MHz SPI Peripheral Clk / 64 =  656,25 kHz SPI Clk
	SPI_InitStructure.FirstBit = SPI_FIRSTBIT_MSB;
	SPI_InitStructure.CRCPolynomial = 7;
//	SPI_Init(SPI3, &SPI_InitStructure);
	ENABLE_SPI3();

}

uint8_t SPI3_TransferByte(uint8_t tx)
{
	uint16_t rx;

	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	while ((SPI3->SR & SPI_I2S_FLAG_TXE) == 0);
	/* Send SPI data */
	SPI3->DR = (uint16_t) tx;
	/* Wait for SPI1 data reception */
	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	while ((SPI3->SR & SPI_I2S_FLAG_RXNE) == 0);
	/* Read SPI received data */
	rx = SPI3->DR;

	return ( (uint8_t) rx);
} /* SPI_TransferByte */

uint16_t SPI3_TransferWord(uint16_t tx)
{
	uint16_t rx;
	while ((SPI3->SR & SPI_I2S_FLAG_TXE) == 0);
	/* Send SPI data */
	SPI3->DR = (uint16_t) tx;
	/* Wait for SPI1 data reception */
	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	while ((SPI3->SR & SPI_I2S_FLAG_RXNE) == 0);
	/* Read SPI received data */
	rx = SPI3->DR;

	return rx;
} /* SPI_TransferWord */

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
