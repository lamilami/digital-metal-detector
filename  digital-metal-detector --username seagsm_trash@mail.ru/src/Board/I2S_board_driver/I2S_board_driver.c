
#include "I2S_board_driver.h"





/*******************************************************************************
* Function Name  : I2S_Configuration
* Description    : I2S2 configuration
* Input          : - I2S_AudioFreq: ²ÉÑùÆµÂÊ
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
* Function Name  : I2S_DMA_Configuration
* Description    : I2S_DMA_Configuration
* Input          : - I2S_AudioFreq: 8000 11000 32000 48000 96000
* Input          : - I2S_Tx_Mode: Master or Slave
* Input          : - MemBaseAddress: pointer to mem buff start
* Input          : - MemBuffSize: memory buffer size
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2S_DMA_Configuration(uint32_t I2S_AudioFreq,uint32_t I2S_Tx_Mode,void* MemBaseAddress, uint32_t MemBuffSize )
{
  I2S_InitTypeDef  I2S_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef  DMA_Init_Structure;

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
  I2S_InitStructure.I2S_Mode = I2S_Tx_Mode;

  I2S_Init(SPI2, &I2S_InitStructure);

  /* Start DMA init. */
  /* DMA1 module is ON. */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* DMA configuration structure. */
  DMA_StructInit(&DMA_Init_Structure);
  DMA_Init_Structure.DMA_PeripheralBaseAddr = (uint32_t)&(SPI2->DR);//set I2S Tx register like target.
  DMA_Init_Structure.DMA_MemoryBaseAddr = (uint32_t)MemBaseAddress;//Set mem buffer.
  DMA_Init_Structure.DMA_DIR = DMA_DIR_PeripheralDST;//data from mem buff to I2S.
  DMA_Init_Structure.DMA_BufferSize = MemBuffSize;//set buffer size.
  DMA_Init_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//disable peripheral address incrimentation.
  DMA_Init_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable;//Enable mem buffer address incrementation.
  DMA_Init_Structure.DMA_Mode = DMA_Mode_Circular;
  DMA_Init_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//set peripheral data size to 16 bits
  DMA_Init_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//set memory data size to 16 bits
  /* Init DMA module. */
  DMA_Init(DMA1_Channel5, &DMA_Init_Structure);

  /* Enable the I2S2 */
  I2S_Cmd(SPI2, ENABLE);
  /* Connect I2S Tx to DMA. */
  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
  /* Start DMA1 Ch5. */
  DMA_Cmd(DMA1_Channel5, ENABLE);



}




/*******************************************************************************
* Function Name  : I2S_INT_Configuration
* Description    : I2S_INT_Configuration
* Input          : - I2S_AudioFreq: 8000 11000 32000 48000 96000
* Input          : - I2S_Tx_Mode: Master or Slave
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2S_INT_Configuration(uint32_t I2S_AudioFreq,uint32_t I2S_Tx_Mode )
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
  I2S_InitStructure.I2S_Mode = I2S_Tx_Mode;

  I2S_Init(SPI2, &I2S_InitStructure);

#if 0
  {
    NVIC_InitTypeDef   NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);//ïðåðûâàíèå åñëè áóôåð ïåðåäàò÷èêà ïóñò(TXE)
  }
#endif



  /* Enable the I2S2 */
  I2S_Cmd(SPI2, ENABLE);
}




/* SPI_I2S interrupt routine. */
void SPI2_IRQHandler(void)
{
  /* Check Tx buffer empty interrupt flag. */
  if( SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE)!= RESET)
  {
    /* Clear Tx buffer empty interrupt flag. */
    SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_TXE);
    /* Write to Tx buffer uint16 value. */
    SPI2->DR=0x0fff;
  }
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
* Input          : - data: WAVÊý¾Ý
*                  - size£ºWAV´óÐ¡
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

  if(userWav.numChannels==1 && userWav.bitsPerSample==8 )         /* WAVµ¥ÉùµÀ 8Î»Á¿»¯ */
  {
	 for(addr=0; addr<size; addr++ )
	 {
	   temp.bits.high=0;
	   temp.bits.low=data[addr];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* ×óÉùµÀÊý¾Ý*/

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* ÓÒÉùµÀÊý¾Ý*/
	 }
  }

  else if(userWav.numChannels==1 && userWav.bitsPerSample==16 )   /* WAVµ¥ÉùµÀ 16Î»Á¿»¯ */
  {
     for(addr=0; addr<size; addr+=2 )
	 {
	   temp.bits.low=data[addr];
	   temp.bits.high=data[addr+1];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* ×óÉùµÀÊý¾Ý*/

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* ÓÒÉùµÀÊý¾Ý*/
	 }
  }
  else if(userWav.numChannels==2 && userWav.bitsPerSample==8 )    /* WAVË«ÉùµÀ 8Î»Á¿»¯ */
  {
     for(addr=0; addr<size; addr++ )
	 {
	   temp.bits.high=0;
	   temp.bits.low=data[addr];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);			/* ÉùµÀÊý¾Ý*/
	 }
  }
  else if(userWav.numChannels==2 && userWav.bitsPerSample==16 )   /* WAVË«ÉùµÀ 16Î»Á¿»¯ */
  {
     for(addr=0; addr<size; addr+=2 )
	 {
	   temp.bits.low=data[addr];
	   temp.bits.high=data[addr+1];

	   /* Wait the Tx buffer to be empty */
       while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) ;
       /* Send a data from I2S3 */
       SPI_I2S_SendData(SPI2, temp.Val);		   /* ÉùµÀÊý¾Ý*/

	 }
  }
}
#endif
