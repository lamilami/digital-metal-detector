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
  uint16_t i,num;
  double b,dac_samples;

  uint16_t wave_data[200];

  delay_init();
  USART_Configuration();
  WM8731_Init();

  /* Build wave data. */
    num = 36;//5-20kHz,10-10kHz,20-5kHz
#if 0
    for(i = 0; i < num; i++)
    {
      b = (double)i*(double)(65000.0/num);
      wave_data[i] = (uint16_t)(b);
    }
#endif
#if 0
    for(i = 0; i < num; i++)
    {
      b = (
           sin( 2*M_PI*((double)i)/((double)num))
          );
      wave_data[i] = (uint16_t)(b*32000.0);
    }
#endif
#if 1
    i=0;
    while(i < (2*num))
    {
      b = (
           sin( 2*M_PI*( (double)i)/( (double)(2.0*num) ))
         + cos( 4*M_PI*( (double)i)/( (double)(2.0*num) ))
         + sin( 6*M_PI*( (double)i)/( (double)(2.0*num) ))
          );
      b=b/3.0;
      wave_data[i] = (uint16_t)(b*32000.0);
      i++;
      wave_data[i] = (uint16_t)(b*32000.0);
      i++;
    }
#endif


  //I2S_Configuration(96000);
  //I2S_INT_Configuration(96000,I2S_Mode_MasterTx );
  I2S_DMA_Configuration(96000, I2S_Mode_MasterTx, wave_data, (2*num));
  i=0;
  while(1)
  {
    //I2S_WriteByte_u16_Direct(wave_data[i]);
    i++;
    if(i>=num)
    {
      i=0;
    }
  }

#if 0
  /* Infinite loop */
  while(1)
  {
    //I2S_WriteByte( (uint8_t*)WAV_DATA , sizeof(WAV_DATA) );
    //I2S_WriteByte_u8_Direct( (uint8_t*)WAV_DATA , sizeof(WAV_DATA) );
    dac_samples = 80;

    for(b = 0; b < 2; b++)
    //for(a = 0; a < dac_samples; a++)
    {
//      b = (
//            sin( 2*M_PI*a/dac_samples)
          //+ cos( 3*M_PI*a/dac_samples)
          //+ cos( 5*M_PI*a/dac_samples)
          //+ cos( 7*M_PI*a/dac_samples)
//          );
//      a=1;
//      while(1)
//      {
//        if(a == 1)
//        {
//          a = -1;
//        }
//        else
//        {
//          a =  1;
//        }
        //data = (uint16_t)b;


        data = (uint16_t)(a*32000.0);
        I2S_WriteByte_u16_Direct(data);
      }
    }
    //delay_us(1000000);
  }
#endif

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
