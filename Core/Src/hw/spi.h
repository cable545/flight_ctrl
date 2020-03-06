
#ifndef __SPI1_H
#define __SPI1_H

void SPI3_Init(void);
uint8_t SPI3_TransferByte(uint8_t tx);
uint16_t SPI3_TransferWord(uint16_t tx);
uint16_t SPI3_Transfer(uint8_t device, uint8_t* txrx, uint16_t length);

#endif
