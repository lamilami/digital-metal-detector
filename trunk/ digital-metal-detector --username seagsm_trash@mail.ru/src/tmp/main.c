/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The Camera (OV7725) application function
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
#include "GLCD.h"
#include "OV7725.h"


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
  delay_init();
  LCD_Initializtion();
  delay_ms(1000);
  FIFO_GPIO_Configuration();
  FIFO_CS_L();

  FIFO_WE_H();
  while( 1 != OV7725_Init() );

  OV7725_EXTI_Configuration();
  OV7725_NVIC_Configuration();

  Vsync = 0;



  /* Infinite loop */
  while (1)
  {
    uint32_t count;
    uint32_t r=0,g=0,b=0,temp;

    uint16_t CMOS_Data;

    if( Vsync == 2 )
    {
       LCD_SetCursor(0, 319);
       LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
       FIFO_RRST_L();
       FIFO_RCLK_L();

       FIFO_RCLK_H();
       FIFO_RRST_H();
       FIFO_RCLK_L();

       FIFO_RCLK_H();

	   for( count = 0; count < 76800; count++ )
        {
          FIFO_RCLK_L();

          CMOS_Data = (GPIOC->IDR<<8) & 0xff00;	  /* ( GPIO_ReadInputData(GPIOC) << 8 ) & 0xff00; */
          FIFO_RCLK_H();

          FIFO_RCLK_L();
          CMOS_Data |= (GPIOC->IDR) & 0x00ff;	  /* ( GPIO_ReadInputData(GPIOC) ) & 0x00ff; */
          FIFO_RCLK_H();

         // CMOS_Data = 0x1F;//blue
         // CMOS_Data = 0x7E0;//green
         // CMOS_Data = 0xF800;//Red
          b = ( CMOS_Data>>0 )  & 0x1f;
          g = ( CMOS_Data>>5 )  & 0x3f;
          r = ( CMOS_Data>>11 ) & 0x1f;
//          temp = (b+r+(g>>1))/3;
//          b = temp;
//          g = temp<<1;
//          r = temp;
//          b=0;
//          g=0;
//          r=0;

          CMOS_Data =  (r<<11) + (g<<5) + (b<<0);
//          if(count < 30000)
//          {
//            CMOS_Data = 0x7800 + 0x3E0 + 0x0F;//Red
//          }
//          else
//          {
//            CMOS_Data = 0xF800 + 0x7E0 + 0x1F;//Red
//          }

          LCD_WriteRAM(CMOS_Data);
         }
         Vsync = 0;
       }
  }
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
