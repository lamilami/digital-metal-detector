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
//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

#include "WM8731.h"
#include "WAV.h"
#include "WavSample.h"
#include "I2S_board_driver.h"
#include "systick.h"
#include "GLCD.h"
#include "TouchPanel.h"
#include "OV7725.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "main.h"

#include "board_GUI_primitives.h"
#include "board_GUI.h"


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private function prototypes -----------------------------------------------*/
void audio_device_init(uint16_t num);
void audio_device_start(void);
void clear_audio_out_buffer(void);
void USART_Configuration(void);
void generate_signal(uint16_t num);
void vTaskLED2(void *pvParameters);
void vTaskLED1(void *pvParameters);

uint16_t wave_data[SIGNAL_BUFFER_SIZE];


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

  uint8_t *str = "TEst_12345678901234567890123456789012345678901234567890";
  uint8_t text_buf[20];
  /* Init system delay.*/
  delay_init();
  /* Init system stdio. */
  USART_Configuration();
  /* Init output signal buffer and audio communication. */
  audio_device_init(SIGNAL_SAMPLES);
  /* start DMA to generate output signal.*/
  audio_device_start();


  /* This part of code should discribe LCD init and LCD GUI */

  LCD_Initializtion();
  LCD_BackLight_Init();
  delay_init();
  TP_Init();
  TouchPanel_Calibrate();

  LCD_Clear(sys_LightGray);


  /* Infinite loop */
/* Usefull function for print test */
#if 0
  /* Print char to XY position. */
  PutCharRotate(160,100,'3',0xF800,0,3);/* Right position.*/
  /* Print string to XY position with normal orientation. */
  GUI_Text(100, 100, str,0xF800, 0);
  /* Print sring to XY position with some another orientation.*/
  GUI_Text_Rotated(100, 140, str,0xF800, 0,3);
#endif


  xTaskCreate( vTaskLED1, ( signed char * ) "LED1", configMINIMAL_STACK_SIZE, NULL, 2,
                        ( xTaskHandle * ) NULL);
  xTaskCreate( vTaskLED2, ( signed char * ) "LED2", configMINIMAL_STACK_SIZE, NULL, 2,
                        ( xTaskHandle * ) NULL);


  LCD_SetPoint(0,0,0xf800);

#if 0
  {
    uint32_t count = 0;
    uint32_t color = 0;

    color=0;
    while(color <= 0xFFFF)
    {
      while(count < 10000)
      {
        LCD_WriteRAM_Prepare();
        LCD_WriteRAM((uint16_t)color);
        //     delay_us(10000);
        count++;
      }
      count = 0 ;
      sprintf(text_buf,"%d",color);
      GUI_Text_Rotated(100, 200, text_buf,0x0, 0xFFFF,3);
      LCD_SetPoint(0,0,(uint16_t)color);
      color++;
    }

  }
#endif
 board_GUI_Init();
while(0)
{
  GuiButtonPressOnOffDraw(&button_1,0);
  delay_us(400000);
  GuiButtonPressOnOffDraw(&button_1,1);
  delay_us(400000);
}




vTaskStartScheduler();


  while (1)
  {
    getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;

    TP_DrawPoint(display.x,display.y);
    TP_DrawPoint(display.x,display.y-1);
    TP_DrawPoint(display.x-1,display.y-1);
    TP_DrawPoint(display.x-1,display.y);
  }

  /* Main loop. */
  while(1)
  {
    delay_us(10000);
  }




}//end of main



void vTaskLED1(void *pvParameters)
{
  uint16_t i,v,y;
  int32_t x;
  i =0;
  v = SIGNAL_BUFFER_SIZE;
  while (1)
  {
    printf("This is task 1\n");
   // vTaskDelay( 500 );

    x = (int32_t)wave_data[i];
    x = x + 32000;
    x = (x*240)/64000;
    i = i+4;
    y = (i * 320)/400;
    TP_DrawPoint((uint16_t)x,y);
    //LCD_SetPoint((uint16_t)x,y,0xf800);
    if(i > v)
    {
      i = 0;
    }
  }

}

void vTaskLED2(void *pvParameters) {

  while (1)
  {
    getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
    TP_DrawPoint(display.x,display.y);
    TP_DrawPoint(display.x,display.y+1);
    TP_DrawPoint(display.x+1,display.y+1);
    TP_DrawPoint(display.x+1,display.y);
    //printf("This is task 2\n");
   // board_GUI_OnTouch(display.x,display.y);
    taskYIELD();
  }

}




/* Generate output signal period. */
void generate_signal(uint16_t num)
{
  uint16_t i;
  double b;

  i=0;
  while(i < (4*num))
  {
    b = (
           sin( 2*M_PI*( (double)i)/( (double)(4.0*num) ))//  1kHz
        //  + sin( 4*M_PI*( (double)i)/( (double)(4.0*num) ))//  2kHz
         + sin( 6*M_PI*( (double)i)/( (double)(4.0*num) ))//  3kHz
//         + sin( 8*M_PI*( (double)i)/( (double)(4.0*num) ))//  4kHz
        //  + sin( 10*M_PI*( (double)i)/( (double)(4.0*num) ))// 5kHz
         + sin( 12*M_PI*( (double)i)/( (double)(4.0*num) ))// 6kHz
        //  + sin( 14*M_PI*( (double)i)/( (double)(4.0*num) ))// 7kHz
//         + sin( 16*M_PI*( (double)i)/( (double)(4.0*num) ))// 8kHz
        //  + sin( 18*M_PI*( (double)i)/( (double)(4.0*num) ))// 9kHz
        //  + sin( 20*M_PI*( (double)i)/( (double)(4.0*num) ))// 10kHz
        //  + sin( 22*M_PI*( (double)i)/( (double)(4.0*num) ))// 11kHz
         + sin( 24*M_PI*( (double)i)/( (double)(4.0*num) ))// 12kHz
//         + sin( 36*M_PI*( (double)i)/( (double)(4.0*num) ))// 18kHz
         );
    //b=b/6.0;
    b=b/100;
    wave_data[i] = (uint16_t)(b*32000.0);//first byte of 32 bits frame Left Ch
    i++;
    wave_data[i] = 0;(uint16_t)(b*32000.0);//second byte of 32 bits frame Left Ch
    i++;
    wave_data[i] = (uint16_t)(b*32000.0);//first byte of 32 bits frame Right Ch
    i++;
    wave_data[i] = 0;(uint16_t)(b*32000.0);//second byte of 32 bits frame Right Ch
    i++;
  }
}

/* Init device. */
void audio_device_init(uint16_t num)
{
  clear_audio_out_buffer();
  generate_signal(num);
  I2S_DMA_Configuration(2, I2S_Mode_SlaveTx, wave_data, (4*num));
  /* WM8731_Init called last to be sure that start bit is valid. */
  WM8731_Init();
}

/* Start audio communication. */
void audio_device_start(void)
{
  I2S_DMA_Communication_Enable();
  WM8731_Active();
}

/* Set to zero audio output buffer. */
void clear_audio_out_buffer(void)
{
   uint16_t i;

  /* Clear signal buffer. */
  i=0;
  while(i < SIGNAL_BUFFER_SIZE)
  {
    wave_data[i] = 0;
    i++;
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
