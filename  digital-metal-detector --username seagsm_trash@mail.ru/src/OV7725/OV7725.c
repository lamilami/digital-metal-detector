/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               OV7725.c
** Descriptions:            OV7725 application function
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

/* Includes ------------------------------------------------------------------*/
#include "SCCB.h"
#include "OV7725.h"
#include "systick.h"

/* Private variables ---------------------------------------------------------*/

volatile uint8_t Vsync;	 /* 帧同步信号 */

const uint8_t OV7725_Reg[REG_NUM][2]=
{
  /* 以下为OV7725QVGA RGB565参数  */
  {0x2a,0x00},{0x11,0x00},
  {0x12,0x46}, /* QVGA RGB565 */
  {0x12,0x46},

  {0x42,0x7f},
  {0x4d,0x00}, /* 0x09 */
  {0x63,0xf0},
  {0x64,0xff},
  {0x65,0x20},
  {0x66,0x00},
  {0x67,0x00},
  {0x69,0x5d},

  {0x13,0xff},
  {0x0d,0xC1}, /* PLL */
  {0x0f,0xc5},
  {0x14,0x11},
  {0x22,0xFF}, /* 7f */
  {0x23,0x01},
  {0x24,0x34},
  {0x25,0x3c},
  {0x26,0xa1},
  {0x2b,0x00},
  {0x6b,0xaa},
  {0x13,0xff},

  {0x90,0x0a},
  {0x91,0x01},
  {0x92,0x01},
  {0x93,0x01},

  {0x94,0x5f},
  {0x95,0x53},
  {0x96,0x11},
  {0x97,0x1a},
  {0x98,0x3d},
  {0x99,0x5a},
  {0x9a,0x1e},

  {0x9b,0x00}, /* set luma */
  {0x9c,0x25}, /* set contrast */
  {0xa7,0x65}, /* set saturation */
  {0xa8,0x65}, /* set saturation */
  {0xa9,0x80}, /* set hue */
  {0xaa,0x80}, /* set hue */

  {0x9e,0x81},
  {0xa6,0x06},

  {0x7e,0x0c},
  {0x7f,0x16},
  {0x80,0x2a},
  {0x81,0x4e},
  {0x82,0x61},
  {0x83,0x6f},
  {0x84,0x7b},
  {0x85,0x86},
  {0x86,0x8e},
  {0x87,0x97},
  {0x88,0xa4},
  {0x89,0xaf},
  {0x8a,0xc5},
  {0x8b,0xd7},
  {0x8c,0xe8},
  {0x8d,0x20},

  {0x33,0x00},
  {0x22,0x99},
  {0x23,0x03},
  {0x4a,0x00},
  {0x49,0x13},
  {0x47,0x08},
  {0x4b,0x14},
  {0x4c,0x17},
  {0x46,0x05},
  {0x0e,0xf5},
  {0x0c,0xd0},

  {0x29,0x50},
  {0x2c,0x78},
};

/*******************************************************************************
* Function Name  : OV7725_XCLK_Init
* Description    : 提供时钟 8MHZ
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void OV7725_XCLK_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  RCC_MCOConfig(RCC_MCO_HSE  );
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : OV7725 GPIO Configuration
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_PORT_VSYNC_CMOS, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  PIN_VSYNC_CMOS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_VSYNC_CMOS, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : FIFO_GPIO_Configuration
* Description    : FIFO GPIO Configuration
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void FIFO_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

  /* FIFO_RCLK : PE1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* FIFO_RRST : PE0 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* FIFO_CS : PD6 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* FIFO_WEN : PD3 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* FIFO_WRST : PB7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* FIFO D[0-7] */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : OV7725_NVIC_Configuration
* Description    : VSYNC中断配置
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void OV7725_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : OV7725_EXTI_Configuration
* Description    : VSYNC中断管脚配置
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void OV7725_EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  GPIO_EXTILineConfig(PORT_SOURCE_VSYNC_CMOS, PIN_SOURCE_VSYNC_CMOS);
  EXTI_InitStructure.EXTI_Line = EXTI_LINE_VSYNC_CMOS;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_LINE_VSYNC_CMOS);
}

/*******************************************************************************
* Function Name  : OV7725_Init
* Description    : OV7725初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 返回1成功，返回0失败
*******************************************************************************/
int OV7725_Init(void)
{
  uint16_t i=0;

  delay_init();

  OV7725_XCLK_Init();

  GPIO_Configuration();

  I2C_Configuration();

  if( 0 == I2C_WriteByte ( 0x12, 0x80 , ADDR_OV7725 ) ) /* Reset SCCB */
  {
     return 0 ;
  }

  delay_ms(50);

  for( i=0 ; i < REG_NUM ; i++ )
  {
	 if( 0 == I2C_WriteByte(  OV7725_Reg[i][0], OV7725_Reg[i][1] , ADDR_OV7725 ) )
	 {
		return 0;
	 }
  }

  return 1;
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
















