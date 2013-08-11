/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			SCCB.h
** Descriptions:		SCCB ²Ù×÷º¯Êý¿â 
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2011-2-13
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef __SCCB_H
#define __SCCB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_10	 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOB->BRR  = GPIO_Pin_10  /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_11	 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOB->BRR  = GPIO_Pin_11	 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL_read      GPIOB->IDR  & GPIO_Pin_10	 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read      GPIOB->IDR  & GPIO_Pin_11	 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */

#define ADDR_OV7725   0x42

/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);
int I2C_Start(void);
void I2C_Stop(void);
int I2C_WaitAck(void);
void I2C_SendByte(uint8_t SendByte); 
int I2C_WriteByte( uint16_t WriteAddress , uint8_t SendByte , uint8_t DeviceAddress);
int I2C_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint16_t ReadAddress,  uint8_t DeviceAddress);

#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
