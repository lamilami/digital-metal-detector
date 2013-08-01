/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               OV7725.h
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-13
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/

#ifndef __OV7725_H
#define __OV7725_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/*------------------------------------------------------
  模块引脚名称 | 描述            |     STM32端引脚连接 |
  ------------------------------------------------------
  SCCB_SCL     : SCCB时钟        : PB10    I2C2_SCL
  SCCB_SDA     : SCCB数据        : PB11    I2C2_SDA
  CAM_VSYNC    : 帧同步          : PA0     外部中断0
  CAM_WRST     : FIFO写地址复位  : PB7     GPIO
  CAM_WEN      : FIFO写允许      : PD3     GPIO
  XCLK         : CMOS传感器主时钟: PA8     MCO输出
  CAM_RRST     : FIFO读地址复位  : PE0     GPIO
  CAM_REN      : FIFO片选        : PD6     GPIO
  CAM_RCLK     : FIFO读时钟      : PE1     GPIO
  FIFO D0~D7   : FIFO数据输出    : PC0~PC7 GPIO
  -----------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define FIFO_CS_PIN     GPIO_Pin_6   /* FIFO片选 */
#define FIFO_WRST_PIN   GPIO_Pin_7   /* FIFO写地址复位 */
#define FIFO_RRST_PIN   GPIO_Pin_0   /* FIFO读地址复位 */
#define FIFO_RCLK_PIN   GPIO_Pin_1   /* FIFO读时钟 */
#define FIFO_WE_PIN     GPIO_Pin_3   /* FIFO写允许 */

#define FIFO_CS_H()     GPIOD->BSRR =FIFO_CS_PIN	  /* GPIO_SetBits(GPIOD , FIFO_CS_PIN)   */
#define FIFO_CS_L()     GPIOD->BRR  =FIFO_CS_PIN	  /* GPIO_ResetBits(GPIOD , FIFO_CS_PIN) */

#define FIFO_WRST_H()   GPIOB->BSRR =FIFO_WRST_PIN	  /* GPIO_SetBits(GPIOB , FIFO_WRST_PIN)   */
#define FIFO_WRST_L()   GPIOB->BRR  =FIFO_WRST_PIN	  /* GPIO_ResetBits(GPIOB , FIFO_WRST_PIN) */

#define FIFO_RRST_H()   GPIOE->BSRR =FIFO_RRST_PIN	  /* GPIO_SetBits(GPIOE , FIFO_RRST_PIN)   */
#define FIFO_RRST_L()   GPIOE->BRR  =FIFO_RRST_PIN	  /* GPIO_ResetBits(GPIOE , FIFO_RRST_PIN) */

#define FIFO_RCLK_H()   GPIOE->BSRR =FIFO_RCLK_PIN	  /* GPIO_SetBits(GPIOE , FIFO_RCLK_PIN)   */
#define FIFO_RCLK_L()   GPIOE->BRR  =FIFO_RCLK_PIN	  /* GPIO_ResetBits(GPIOE , FIFO_RCLK_PIN) */

#define FIFO_WE_H()     GPIOD->BSRR =FIFO_WE_PIN	  /* GPIO_SetBits(GPIOD , FIFO_WE_PIN)   */
#define FIFO_WE_L()     GPIOD->BRR  =FIFO_WE_PIN	  /* GPIO_ResetBits(GPIOD , FIFO_WE_PIN) */

#define REG_NUM                            74
#define PORT_VSYNC_CMOS                    GPIOA
#define RCC_APB2Periph_PORT_VSYNC_CMOS     RCC_APB2Periph_GPIOA
#define PIN_VSYNC_CMOS                     GPIO_Pin_0
#define EXTI_LINE_VSYNC_CMOS               EXTI_Line0
#define PORT_SOURCE_VSYNC_CMOS             GPIO_PortSourceGPIOA
#define PIN_SOURCE_VSYNC_CMOS              GPIO_PinSource0

/* Private variables ---------------------------------------------------------*/
extern volatile uint8_t Vsync;

/* Private function prototypes -----------------------------------------------*/
int  OV7725_Init(void);
void FIFO_GPIO_Configuration(void);
void OV7725_NVIC_Configuration(void);
void OV7725_EXTI_Configuration(void);
int  OV7725_ReadReg(uint8_t LCD_Reg,uint16_t LCD_RegValue);
int  OV7725_WriteReg(uint8_t LCD_Reg,uint16_t LCD_RegValue);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






























