#ifndef __I2S_BOARD_DRIVER_H
#define __I2S_BOARD_DRIVER_H

#include "stm32f10x.h"
#include "stm32f10x_dma.h"

void I2S_Configuration(uint32_t I2S_AudioFreq);
void I2S_INT_Configuration(uint32_t I2S_AudioFreq,uint32_t I2S_Tx_Mode );
void I2S_DMA_Configuration(
                            uint32_t I2S_AudioFreq,
                            uint32_t I2S_Tx_Mode,
                            void* MemBaseAddress,
                            uint32_t MemBuffSize
                          );
void I2S_DMA_Communication_Enable(void);
void I2S_WriteByte( uint8_t * data , uint32_t size);
void I2S_WriteByte_u8_Direct( uint8_t * data , uint32_t size);
void I2S_WriteByte_u16_Direct(uint16_t data);





#endif