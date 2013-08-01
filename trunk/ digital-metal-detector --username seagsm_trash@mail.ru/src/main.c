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
#include "GLCD.h"
#include "OV7725.h"
#include <stdio.h>
#include <string.h>


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private function prototypes -----------------------------------------------*/
void USART_Configuration(void);
void I2S_Configuration(uint32_t I2S_AudioFreq);
void I2S_WriteByte( uint8_t * data , uint32_t size);

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
  USART_Configuration();
  WM8731_Init();
  memcpy(&userWav,WAV_DATA,sizeof(WavHeader));
  DisplayWavInfo();
  I2S_Configuration(userWav.sampleRate);
  /* Infinite loop */
  while(1)
  {
    I2S_WriteByte( (uint8_t*)WAV_DATA , sizeof(WAV_DATA) );
  }  
}

/*******************************************************************************
* Function Name  : I2S_Configuration
* Description    : I2S2 configuration
* Input          : - I2S_AudioFreq: 采样频率
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2S_Configuration(uint32_t I2S_AudioFreq)
{
  I2S_InitTypeDef I2S_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); 

  /* WS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* CK	*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* SD	*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  SPI_I2S_DeInit(SPI2);
							
  I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;

  /* I2S3 Master Transmitter to I2S2 Slave Receiver communication ------------*/
  /* I2S3 configuration */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;

  I2S_Init(SPI2, &I2S_InitStructure);

  /* Enable the I2S2 */
  I2S_Cmd(SPI2, ENABLE);
}

/*******************************************************************************
* Function Name  : I2S_WriteByte
* Description    : I2S To WM8731
* Input          : - data: WAV数据
*                  - size：WAV大小
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2S_WriteByte( uint8_t * data , uint32_t size)
{
  typedef union 
  {
     uint16_t Val;
     struct
      { 
        uint16_t low	 :8;				
        uint16_t high	 :8;			
      }bits;	    
  }wav;
  
  uint32_t addr;
  wav temp;	

  if(userWav.numChannels==1 && userWav.bitsPerSample==8 )         /* WAV单声道 8位量化 */
  {
	 for(addr=0; addr<size; addr++ )
	 {
	   temp.bits.high=0;
	   temp.bits.low=data[addr];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* 左声道数据*/

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* 右声道数据*/
	 } 
  }

  else if(userWav.numChannels==1 && userWav.bitsPerSample==16 )   /* WAV单声道 16位量化 */
  {
     for(addr=0; addr<size; addr+=2 )
	 { 
	   temp.bits.low=data[addr];
	   temp.bits.high=data[addr+1];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* 左声道数据*/

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* 右声道数据*/
	 } 
  }
  else if(userWav.numChannels==2 && userWav.bitsPerSample==8 )    /* WAV双声道 8位量化 */
  {
     for(addr=0; addr<size; addr++ )
	 {
	   temp.bits.high=0;
	   temp.bits.low=data[addr];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* 声道数据*/
	 } 
  }
  else if(userWav.numChannels==2 && userWav.bitsPerSample==16 )   /* WAV双声道 16位量化 */
  {
     for(addr=0; addr<size; addr+=2 )
	 { 
	   temp.bits.low=data[addr];
	   temp.bits.high=data[addr+1];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);		   /* 声道数据*/

	 } 
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
