/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			GLCD.c
** Descriptions:		STM32 FSMC TFT操作函数库
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

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h" 

/* Private variables ---------------------------------------------------------*/
static uint16_t DeviceCode;

/* Private define ------------------------------------------------------------*/
/* 使用总线方式时定义地址 */
/* 挂在不同的BANK,使用不同地址线时请自行换算地址 */
#define LCD_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */


/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
                        
  /* PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9),
     PE.13(D10), PE.14(D11), PE.15(D12) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                 GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* PD.00(D2), PD.01(D3), PD.04(RD), PD.5(WR), PD.7(CS), PD.8(D13), PD.9(D14),
     PD.10(D15), PD.11(RS) PD.14(D0) PD.15(D1) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | 
                                 GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | 
                                 GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
  /* FSMC读速度设置 */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 30;  /* 地址建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 30;	   /* 数据建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  /* FSMC写速度设置 */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 2;   /* 地址建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 2;	   /* 数据建立时间  */
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;	  

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}


/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
 /* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static __inline void LCD_WriteReg(uint8_t LCD_Reg,uint16_t LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
static __inline uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD_RAM);
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{		  
  LCD_REG = R34;
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteRAM(uint16_t RGB_Code)					 
{
  /* Write 16-bit GRAM Reg */
  LCD_RAM = RGB_Code;
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
* Attention		 : None
*******************************************************************************/
static __inline uint16_t LCD_ReadRAM(void)
{
  volatile uint16_t dummy; 
  /* Write 16-bit Index (then Read Reg) */
  LCD_REG = R34; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  dummy = LCD_RAM; 
  
  return LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetCursor(uint16_t Xpos,uint16_t Ypos)
{
  if(DeviceCode==0x8989)
  {
    LCD_WriteReg(0x004e,Xpos); /* Row */
    LCD_WriteReg(0x004f,Ypos); /* Line */ 
  }
  else if(DeviceCode==0x9919)
  {
    LCD_WriteReg(0x004e,Xpos); /* Row */
    LCD_WriteReg(0x004f,Ypos); /* Line */	
  }
  else
  {
    LCD_WriteReg(0x0020,Xpos); /* Row */
    LCD_WriteReg(0x0021,Ypos); /* Line */
  }
}

/*******************************************************************************
* Function Name  : LCD_Delay
* Description    : Delay Time
* Input          : - nCount: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Delay(uint16_t nCount)
{
 uint16_t TimingDelay; 
 while(nCount--)
   {
    for(TimingDelay=0;TimingDelay<10000;TimingDelay++);
   }
}

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : Initialize TFT Controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Initializtion()
{
  LCD_Configuration();
  LCD_Delay(5);  /* delay 50 ms */		

  DeviceCode = LCD_ReadReg(0x0000);		/* 读取屏ID	*/
  if( DeviceCode==0x8989 )	/* 不同屏驱动IC 初始化不同 */
  {
     LCD_WriteReg(0x0007,0x00021);
     /* set R00h at 0001h (OSCEN=1)	*/
     LCD_WriteReg(0x0000,0x0001);

     LCD_WriteReg(0x0010,0x0000);
     LCD_Delay(5);  /* delay 50 ms */	
     /* set R07h at 0033h (GON=1,DTE=1,D[1:0]=11) */
     LCD_WriteReg(0x0007,0x0033);
     LCD_WriteReg(0x0011,0x6058);
     /* LCD driver AC setting (R02h) */
     LCD_WriteReg(0x0002,0x0600);
     LCD_WriteReg(0x0003,0xc885);  
     LCD_WriteReg(0x000C,0x0004);
     LCD_WriteReg(0x000D,0x0009);

     LCD_WriteReg(0x000E,0x2900);
     LCD_WriteReg(0x001E,0x00B7); /* VCOM */
     LCD_WriteReg(0x0001,0x2B3F); /*驱动输出控制320*240 */
     LCD_WriteReg(0x0010,0x0000);
     LCD_WriteReg(0x0005,0x0000);
     LCD_WriteReg(0x0006,0x0000);
     LCD_WriteReg(0x0016,0xEF1C);
     LCD_WriteReg(0x0017,0x0003);
     LCD_WriteReg(0x0007,0x0233); 
     LCD_WriteReg(0x000B,0x5308);
     LCD_WriteReg(0x000F,0x0000); /*扫描开始地址	*/
     LCD_WriteReg(0x0041,0x0000);
     LCD_WriteReg(0x0042,0x0000);
     LCD_WriteReg(0x0048,0x0000);
     LCD_WriteReg(0x0049,0x013F);
     LCD_WriteReg(0x004A,0x0000);
     LCD_WriteReg(0x004B,0x0000);
     LCD_WriteReg(0x0044,0xEF00);
     LCD_WriteReg(0x0045,0x0000);
     LCD_WriteReg(0x0046,0x013F);
     /*         gamma          */
     LCD_WriteReg(0x0030,0x0101);
     LCD_WriteReg(0x0031,0x0204);
     LCD_WriteReg(0x0032,0x0204);
     LCD_WriteReg(0x0033,0x0502);
     LCD_WriteReg(0x0034,0x0707);
     LCD_WriteReg(0x0035,0x0204);
     LCD_WriteReg(0x0036,0x0204);
     LCD_WriteReg(0x0037,0x0502);
     LCD_WriteReg(0x003A,0x0307);
     LCD_WriteReg(0x003B,0x0601);

     LCD_WriteReg(0x0023,0x0000);
     LCD_WriteReg(0x0024,0x0000);
     LCD_WriteReg(0x0025,0x8000);   
     LCD_WriteReg(0x004f,0);        
     LCD_WriteReg(0x004e,0);      
  }	
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

