#ifndef __I2S_BOARD_DRIVER_H
#define __I2S_BOARD_DRIVER_H

#include "stm32f10x.h"

void I2S_Configuration(uint32_t I2S_AudioFreq);
void I2S_WriteByte( uint8_t * data , uint32_t size);
void I2S_WriteByte_u8_Direct( uint8_t * data , uint32_t size);
void I2S_WriteByte_u16_Direct(uint16_t data);





#endif