#include "nrf24l01.h"
#include "spi.h"
#include "usart.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; // send model address
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; // receive model address


//
void NRF24L01_SPI_Init(void)
{
    __HAL_SPI_DISABLE(&hspi3);               //ÏÈ¹Ø±ÕSPI2
    hspi3.Init.CLKPolarity=SPI_POLARITY_LOW; //´®ÐÐÍ¬²½Ê±ÖÓµÄ¿ÕÏÐ×´Ì¬ÎªµÍµçÆ½
    hspi3.Init.CLKPhase=SPI_PHASE_1EDGE;     //´®ÐÐÍ¬²½Ê±ÖÓµÄµÚ1¸öÌø±äÑØ£¨ÉÏÉý»òÏÂ½µ£©Êý¾Ý±»²ÉÑù
    HAL_SPI_Init(&hspi3);
    __HAL_SPI_ENABLE(&hspi3);                //Ê¹ÄÜSPI2
}


/**
 * @brief This function handles Memory management fault.
 */
void Init_NRF24L01(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();


#if 0
	GPIO_InitStruct.Pin = NRF_CS;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = NRF_CE;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB12 */
	GPIO_InitStruct.Pin = NRF_IRQ;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	// GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOB, NRF_CS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, NRF_CE, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOB, NRF_IRQ, GPIO_PIN_SET);

	// MX_SPI3_Init();
	// NRF24L01_SPI_Init();
	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif

	// CE
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// CS
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// SCK
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// MOSI
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// MISO
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// IRQ
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	CE_H;  // ³õÊ¼»¯Ê±ÏÈÀ­¸ß
	CSN_H; // ³õÊ¼»¯Ê±ÏÈÀ­¸ß

	CE_L;  // Ê¹ÄÜNRF24L01
	CSN_H; // SPIÆ¬Ñ¡È¡Ïû
}


void SOFT_SPI_Delay(uint8_t us)
{
        for(int i = 0; i < us; i++)    
        {
            __asm("NOP");//core bus 160M  Çé¿öÏÂ´ó¸ÅIICËÙÂÊ 400K
        }
    
}

//Ä£ÄâSPI¶ÁÐ´Êý¾Ýº¯Êý
uint8_t SPI_ReadWriteByte(uint8_t TxData)                                        
{		
	uint16_t bit_ctr;
   	for(bit_ctr=0;bit_ctr<8;bit_ctr++) 
   	{
		if(TxData & 0x80)
		MOSI_H;         
		else
		MOSI_L;
		TxData = (TxData << 1);           
		SCK_H; 
		SOFT_SPI_Delay(10);
		if(READ_MISO)                     
		TxData |= 0x01;       		  
		SCK_L; 
		SOFT_SPI_Delay(10);          		 
   	}
    return(TxData);           		  		    
}

	 
//Í¨¹ýSPIÐ´¼Ä´æÆ÷
uint8_t NRF24L01_Write_Reg(uint8_t reg_addr,uint8_t data)
{
	uint8_t status;	
    CSN_L;                    //Ê¹ÄÜSPI´«Êä
  	status =SPI_ReadWriteByte(reg_addr); //·¢ËÍ¼Ä´æÆ÷ºÅ 
  	SPI_ReadWriteByte(data);            //Ð´Èë¼Ä´æÆ÷µÄÖµ
  	CSN_H;                    //½ûÖ¹SPI´«Êä	   
  	return(status);       		         //·µ»Ø×´Ì¬Öµ
}
//¶ÁÈ¡SPI¼Ä´æÆ÷Öµ £¬regaddr:Òª¶ÁµÄ¼Ä´æÆ÷
uint8_t NRF24L01_Read_Reg(uint8_t reg_addr)
{
	uint8_t reg_val;	    
 	CSN_L;                //Ê¹ÄÜSPI´«Êä		
  	SPI_ReadWriteByte(reg_addr);     //·¢ËÍ¼Ä´æÆ÷ºÅ
  	reg_val=SPI_ReadWriteByte(0);//¶ÁÈ¡¼Ä´æÆ÷ÄÚÈÝ

  	CSN_H;                //½ûÖ¹SPI´«Êä		    
  	return(reg_val);                 //·µ»Ø×´Ì¬Öµ
}	
//ÔÚÖ¸¶¨Î»ÖÃ¶Á³öÖ¸¶¨³¤¶ÈµÄÊý¾Ý
//*pBuf:Êý¾ÝÖ¸Õë
//·µ»ØÖµ,´Ë´Î¶Áµ½µÄ×´Ì¬¼Ä´æÆ÷Öµ 
uint8_t NRF24L01_Read_Buf(uint8_t reg_addr,uint8_t *pBuf,uint8_t data_len)
{
	uint8_t status,i;	       
  	CSN_L;                     //Ê¹ÄÜSPI´«Êä
  	status=SPI_ReadWriteByte(reg_addr);   //·¢ËÍ¼Ä´æÆ÷Öµ(Î»ÖÃ),²¢¶ÁÈ¡×´Ì¬Öµ   	   
 	for(i=0;i<data_len;i++)
		pBuf[i]=SPI_ReadWriteByte(0);//¶Á³öÊý¾Ý

  	CSN_H;                     //¹Ø±ÕSPI´«Êä
  	return status;                        //·µ»Ø¶Áµ½µÄ×´Ì¬Öµ
}
//ÔÚÖ¸¶¨Î»ÖÃÐ´Ö¸¶¨³¤¶ÈµÄÊý¾Ý
//*pBuf:Êý¾ÝÖ¸Õë
//·µ»ØÖµ,´Ë´Î¶Áµ½µÄ×´Ì¬¼Ä´æÆ÷Öµ
uint8_t NRF24L01_Write_Buf(uint8_t reg_addr, uint8_t *pBuf, uint8_t data_len)
{
	uint8_t status,i;	    
 	CSN_L;                                    //Ê¹ÄÜSPI´«Êä
  	status = SPI_ReadWriteByte(reg_addr);                //·¢ËÍ¼Ä´æÆ÷Öµ(Î»ÖÃ),²¢¶ÁÈ¡×´Ì¬Öµ
  	for(i=0; i<data_len; i++)
		SPI_ReadWriteByte(*pBuf++); //Ð´ÈëÊý¾Ý	 
  	CSN_H;                                    //¹Ø±ÕSPI´«Êä
  	return status;                                       //·µ»Ø¶Áµ½µÄ×´Ì¬Öµ
}				   


//ÉÏµç¼ì²âNRF24L01ÊÇ·ñÔÚÎ»
//Ð´5¸öÊý¾ÝÈ»ºóÔÙ¶Á»ØÀ´½øÐÐ±È½Ï£¬
//ÏàÍ¬Ê±·µ»ØÖµ0£¬±íÊ¾ÔÚÎ»;
//·ñÔò·µ»Ø1£¬±íÊ¾²»ÔÚÎ».

uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	uint8_t buf1[5];
	uint8_t i;
	NRF24L01_Write_Buf(SPI_WRITE_REG + TX_ADDR, buf, 5); // Ð´Èë5¸ö×Ö½ÚµÄµØÖ·.
	NRF24L01_Read_Buf(TX_ADDR, buf1, 5);				 // ¶Á³öÐ´ÈëµÄµØÖ·
	for (i = 0; i < 5; i++)
		if (buf1[i] != 0XA5)
		break;
	if (i != 5)
		return 1; // NRF24L01²»ÔÚÎ»

	return 0; // NRF24L01ÔÚÎ»
}

//Æô¶¯NRF24L01·¢ËÍÒ»´ÎÊý¾Ý
//txbuf:´ý·¢ËÍÊý¾ÝÊ×µØÖ·
//·µ»ØÖµ:·¢ËÍÍê³É×´¿ö
uint8_t NRF24L01_TxPacket(uint8_t *tx_buf)
{
	uint8_t state;   
	// printf("00\r\n");
	CE_L;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);//Ð´Êý¾Ýµ½TX BUF  32¸ö×Ö½Ú
 	CE_H;                                     //Æô¶¯·¢ËÍ	   
	// printf("01\r\n");
	while(READ_IRQ != 0);                         //µÈ´ý·¢ËÍÍê³É
	// printf("02\r\n");
	state=NRF24L01_Read_Reg(STATUS);                     //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ	   
	// printf("03 0x%x\r\n",state);
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state);      //Çå³ýTX_DS»òMAX_RTÖÐ¶Ï±êÖ¾


	if(state&MAX_TX)                                     //´ïµ½×î´óÖØ·¢´ÎÊý
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);               //Çå³ýTX FIFO¼Ä´æÆ÷ 
		return MAX_TX; 
	}
	if(state&TX_OK)                                      //·¢ËÍÍê³É
	{
		return TX_OK;
	}

	return 0xff;                                         //ÆäËûÔ­Òò·¢ËÍÊ§°Ü
}

//Æô¶¯NRF24L01·¢ËÍÒ»´ÎÊý¾Ý
//txbuf:´ý·¢ËÍÊý¾ÝÊ×µØÖ·
//·µ»ØÖµ:0£¬½ÓÊÕÍê³É£»ÆäËû£¬´íÎó´úÂë
uint8_t NRF24L01_RxPacket(uint8_t *rx_buf)
{
	uint8_t state;		    							      
	state=NRF24L01_Read_Reg(STATUS);                //¶ÁÈ¡×´Ì¬¼Ä´æÆ÷µÄÖµ    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,state); //Çå³ýTX_DS»òMAX_RTÖÐ¶Ï±êÖ¾
	if(state&RX_OK)                                 //½ÓÊÕµ½Êý¾Ý
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);//¶ÁÈ¡Êý¾Ý
		NRF24L01_Write_Reg(FLUSH_RX,0xff);          //Çå³ýRX FIFO¼Ä´æÆ÷ 
		return 0; 
	}	   
	return 1;                                      //Ã»ÊÕµ½ÈÎºÎÊý¾Ý
}



//¸Ãº¯Êý³õÊ¼»¯NRF24L01µ½RXÄ£Ê½
//ÉèÖÃRXµØÖ·,Ð´RXÊý¾Ý¿í¶È,Ñ¡ÔñRFÆµµÀ,²¨ÌØÂÊºÍLNA HCURR
//µ±CE±ä¸ßºó,¼´½øÈëRXÄ£Ê½,²¢¿ÉÒÔ½ÓÊÕÊý¾ÝÁË		   
void RX_Mode(void)
{
	CE_L;	  
    //Ð´RX½ÚµãµØÖ·
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);

    //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ð    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    
    //Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·  	 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);
    //ÉèÖÃRFÍ¨ÐÅÆµÂÊ		  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);	     
    //Ñ¡ÔñÍ¨µÀ0µÄÓÐÐ§Êý¾Ý¿í¶È 	    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
    //ÉèÖÃTX·¢Éä²ÎÊý,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);
    //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊý;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX½ÓÊÕÄ£Ê½ 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x0f); 
    //CEÎª¸ß,½øÈë½ÓÊÕÄ£Ê½ 
  	CE_H;                                
}			

//¸Ãº¯Êý³õÊ¼»¯NRF24L01µ½TXÄ£Ê½
//ÉèÖÃTXµØÖ·,Ð´TXÊý¾Ý¿í¶È,ÉèÖÃRX×Ô¶¯Ó¦´ðµÄµØÖ·,Ìî³äTX·¢ËÍÊý¾Ý,
//Ñ¡ÔñRFÆµµÀ,²¨ÌØÂÊºÍLNA HCURR PWR_UP,CRCÊ¹ÄÜ
//µ±CE±ä¸ßºó,¼´½øÈëRXÄ£Ê½,²¢¿ÉÒÔ½ÓÊÕÊý¾ÝÁË		   
//CEÎª¸ß´óÓÚ10us,ÔòÆô¶¯·¢ËÍ.	 
void TX_Mode(void)
{														 
	CE_L;	    
    //Ð´TX½ÚµãµØÖ· 
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);    
    //ÉèÖÃTX½ÚµãµØÖ·,Ö÷ÒªÎªÁËÊ¹ÄÜACK	  
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); 

    //Ê¹ÄÜÍ¨µÀ0µÄ×Ô¶¯Ó¦´ð    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     
    //Ê¹ÄÜÍ¨µÀ0µÄ½ÓÊÕµØÖ·  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); 
    //ÉèÖÃ×Ô¶¯ÖØ·¢¼ä¸ôÊ±¼ä:500us + 86us;×î´ó×Ô¶¯ÖØ·¢´ÎÊý:10´Î
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x1a);
    //ÉèÖÃRFÍ¨µÀÎª40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,40);       
    //ÉèÖÃTX·¢Éä²ÎÊý,0dbÔöÒæ,2Mbps,µÍÔëÉùÔöÒæ¿ªÆô   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x0f);  
    //ÅäÖÃ»ù±¾¹¤×÷Ä£Ê½µÄ²ÎÊý;PWR_UP,EN_CRC,16BIT_CRC,PRIM_RX·¢ËÍÄ£Ê½,¿ªÆôËùÓÐÖÐ¶Ï
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);    
    // CEÎª¸ß,10usºóÆô¶¯·¢ËÍ
	CE_H;                                  
}	