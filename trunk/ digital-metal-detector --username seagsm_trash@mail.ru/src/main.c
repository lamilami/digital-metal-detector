/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The WM8731 application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-17
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
#include "stm32f10x.h"
#include "WM8731.h"
#include "WAV.h"
#include "WavSample.h"
#include "I2S_board_driver.h"
#include "systick.h"
#include "GLCD.h"
#include "OV7725.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define M_PI 3.1415


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private function prototypes -----------------------------------------------*/
void USART_Configuration(void);

uint16_t wave_data[200];


/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main(void)
{
  uint16_t i,num,v;
  double b,dac_samples;

  /* Clear mem buffer. */
  i=0;
  while(i<200)
  {
    wave_data[i] = 0;
    i++;
  }
  delay_init();
  USART_Configuration();

  /* Build wave data. */
    num = 96;//5-20kHz,10-10kHz,20-5kHz
#if 1
    i=0;
    while(i < (4*num))
    {
      b = (
        //   sin( 2*M_PI*( (double)i)/( (double)(4.0*num) ))//  1kHz
        //  + sin( 4*M_PI*( (double)i)/( (double)(4.0*num) ))//  2kHz
         + sin( 6*M_PI*( (double)i)/( (double)(4.0*num) ))//  3kHz
         + sin( 8*M_PI*( (double)i)/( (double)(4.0*num) ))//  4kHz
        //  + sin( 10*M_PI*( (double)i)/( (double)(4.0*num) ))// 5kHz
         + sin( 12*M_PI*( (double)i)/( (double)(4.0*num) ))// 6kHz
        //  + sin( 14*M_PI*( (double)i)/( (double)(4.0*num) ))// 7kHz
         + sin( 16*M_PI*( (double)i)/( (double)(4.0*num) ))// 8kHz
        //  + sin( 18*M_PI*( (double)i)/( (double)(4.0*num) ))// 9kHz
        //  + sin( 20*M_PI*( (double)i)/( (double)(4.0*num) ))// 10kHz
        //  + sin( 22*M_PI*( (double)i)/( (double)(4.0*num) ))// 11kHz
         + sin( 24*M_PI*( (double)i)/( (double)(4.0*num) ))// 12kHz
         + sin( 36*M_PI*( (double)i)/( (double)(4.0*num) ))// 18kHz
           );
      b=b/6.0;
      wave_data[i] = (uint16_t)(b*32000.0);//first byte of 32 bits frame Left Ch
      i++;
      wave_data[i] = 0;(uint16_t)(b*32000.0);//second byte of 32 bits frame Left Ch
      i++;
      wave_data[i] = (uint16_t)(b*32000.0);//first byte of 32 bits frame Right Ch
      i++;
      wave_data[i] = 0;(uint16_t)(b*32000.0);//second byte of 32 bits frame Right Ch
      i++;
    }
#endif
  I2S_DMA_Configuration(2, I2S_Mode_SlaveTx, wave_data, (4*num));
  /* This way of configuration is implemented to be sure that start bit is valid. */
  WM8731_Init();
  I2S_DMA_Communication_Enable();
  WM8731_Active();

  i=0;
  v=0;
  while(1)
  {
    i=0;
    while(i<200)
    {
    //  wave_data[i] = v;
      i++;
    }
    v++;
    delay_us(10000);

    //I2S_WriteByte_u16_Direct(wave_data[i]);
    //i++;
    //if(i>=num)
    //{
    //  i=0;
    //}
  }

}









/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configure USART1
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX -> PA10
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_Cmd(USART1, ENABLE);
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
