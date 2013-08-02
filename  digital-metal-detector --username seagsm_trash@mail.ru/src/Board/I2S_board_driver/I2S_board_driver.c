
#include "I2S_board_driver.h"





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
* Input          : data: u16
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void I2S_WriteByte_u16_Direct(uint16_t data)
{
    /* Wait the Tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
    /* Send a data from I2S3 */
    SPI_I2S_SendData(SPI2, data);			/* to Left CH */

    /* Wait the Tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
    /* Send a data from I2S3 */
    SPI_I2S_SendData(SPI2, data);			/* to Right CH */
}

/*******************************************************************************
* Function Name  : I2S_WriteByte
* Description    : I2S To WM8731
* Input          : - data: pionter to u8
*                  - size: Any
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void I2S_WriteByte_u8_Direct( uint8_t * data , uint32_t size)
{
  uint32_t addr;
  uint8_t temp;
  for(addr=0; addr<size; addr++ )
  {
    temp=data[addr];

    /* Wait the Tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
    /* Send a data from I2S3 */
    SPI_I2S_SendData(SPI2, temp<<7);			/* to Left CH */

    /* Wait the Tx buffer to be empty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
    /* Send a data from I2S3 */
    SPI_I2S_SendData(SPI2, temp<<7);			/* to Right CH */
  }
}

#if 0 // This code is disabled. Keeped for example.
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
#endif
