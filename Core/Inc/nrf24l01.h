#ifndef _NRF24L01_H
#define _NRF24L01_H

#include "main.h"
/****************************************************************************************************/
//NRF24L01¼Ä´æÆ÷²Ù×÷ÃüÁî
#define SPI_READ_REG    0x00  //¶ÁÅäÖÃ¼Ä´æÆ÷,µÍ5Î»Îª¼Ä´æÆ÷µØÖ·
#define SPI_WRITE_REG   0x20  //Ð´ÅäÖÃ¼Ä´æÆ÷,µÍ5Î»Îª¼Ä´æÆ÷µØÖ·
#define RD_RX_PLOAD     0x61  //¶ÁRXÓÐÐ§Êý¾Ý,1~32×Ö½Ú
#define WR_TX_PLOAD     0xA0  //Ð´TXÓÐÐ§Êý¾Ý,1~32×Ö½Ú
#define FLUSH_TX        0xE1  //Çå³ýTX FIFO¼Ä´æÆ÷.·¢ÉäÄ£Ê½ÏÂÓÃ
#define FLUSH_RX        0xE2  //Çå³ýRX FIFO¼Ä´æÆ÷.½ÓÊÕÄ£Ê½ÏÂÓÃ
#define REUSE_TX_PL     0xE3  //ÖØÐÂÊ¹ÓÃÉÏÒ»°üÊý¾Ý,CEÎª¸ß,Êý¾Ý°ü±»²»¶Ï·¢ËÍ.
#define NOP             0xFF  //¿Õ²Ù×÷,¿ÉÒÔÓÃÀ´¶Á×´Ì¬¼Ä´æÆ÷	
 
//SPI(NRF24L01)¼Ä´æÆ÷µØÖ·
#define CONFIG          0x00  //ÅäÖÃ¼Ä´æÆ÷µØÖ·;bit0:1½ÓÊÕÄ£Ê½,0·¢ÉäÄ£Ê½;bit1:µçÑ¡Ôñ;bit2:CRCÄ£Ê½;bit3:CRCÊ¹ÄÜ;
                              //bit4:ÖÐ¶ÏMAX_RT(´ïµ½×î´óÖØ·¢´ÎÊýÖÐ¶Ï)Ê¹ÄÜ;bit5:ÖÐ¶ÏTX_DSÊ¹ÄÜ;bit6:ÖÐ¶ÏRX_DRÊ¹ÄÜ
#define EN_AA           0x01  //Ê¹ÄÜ×Ô¶¯Ó¦´ð¹¦ÄÜ  bit0~5,¶ÔÓ¦Í¨µÀ0~5
#define EN_RXADDR       0x02  //½ÓÊÕµØÖ·ÔÊÐí,bit0~5,¶ÔÓ¦Í¨µÀ0~5
#define SETUP_AW        0x03  //ÉèÖÃµØÖ·¿í¶È(ËùÓÐÊý¾ÝÍ¨µÀ):bit1,0:00,3×Ö½Ú;01,4×Ö½Ú;02,5×Ö½Ú;
#define SETUP_RETR      0x04  //½¨Á¢×Ô¶¯ÖØ·¢;bit3:0,×Ô¶¯ÖØ·¢¼ÆÊýÆ÷;bit7:4,×Ô¶¯ÖØ·¢ÑÓÊ± 250*x+86us
#define RF_CH           0x05  //RFÍ¨µÀ,bit6:0,¹¤×÷Í¨µÀÆµÂÊ;
#define RF_SETUP        0x06  //RF¼Ä´æÆ÷;bit3:´«ÊäËÙÂÊ(0:1Mbps,1:2Mbps);bit2:1,·¢Éä¹¦ÂÊ;bit0:µÍÔëÉù·Å´óÆ÷ÔöÒæ
#define STATUS          0x07  //×´Ì¬¼Ä´æÆ÷;bit0:TX FIFOÂú±êÖ¾;bit3:1,½ÓÊÕÊý¾ÝÍ¨µÀºÅ(×î´ó:6);bit4,´ïµ½×î¶à´ÎÖØ·¢
                              //bit5:Êý¾Ý·¢ËÍÍê³ÉÖÐ¶Ï;bit6:½ÓÊÕÊý¾ÝÖÐ¶Ï;
#define MAX_TX  	    0x10  //´ïµ½×î´ó·¢ËÍ´ÎÊýÖÐ¶Ï
#define TX_OK       	0x20  //TX·¢ËÍÍê³ÉÖÐ¶Ï
#define RX_OK   	    0x40  //½ÓÊÕµ½Êý¾ÝÖÐ¶Ï

#define OBSERVE_TX      0x08  //·¢ËÍ¼ì²â¼Ä´æÆ÷,bit7:4,Êý¾Ý°ü¶ªÊ§¼ÆÊýÆ÷;bit3:0,ÖØ·¢¼ÆÊýÆ÷
#define CD              0x09  //ÔØ²¨¼ì²â¼Ä´æÆ÷,bit0,ÔØ²¨¼ì²â;
#define RX_ADDR_P0      0x0A  //Êý¾ÝÍ¨µÀ0½ÓÊÕµØÖ·,×î´ó³¤¶È5¸ö×Ö½Ú,µÍ×Ö½ÚÔÚÇ°
#define RX_ADDR_P1      0x0B  //Êý¾ÝÍ¨µÀ1½ÓÊÕµØÖ·,×î´ó³¤¶È5¸ö×Ö½Ú,µÍ×Ö½ÚÔÚÇ°
#define RX_ADDR_P2      0x0C  //Êý¾ÝÍ¨µÀ2½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define RX_ADDR_P3      0x0D  //Êý¾ÝÍ¨µÀ3½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define RX_ADDR_P4      0x0E  //Êý¾ÝÍ¨µÀ4½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define RX_ADDR_P5      0x0F  //Êý¾ÝÍ¨µÀ5½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define TX_ADDR         0x10  //·¢ËÍµØÖ·(µÍ×Ö½ÚÔÚÇ°),ShockBurstTMÄ£Ê½ÏÂ,RX_ADDR_P0Óë´ËµØÖ·ÏàµÈ
#define RX_PW_P0        0x11  //½ÓÊÕÊý¾ÝÍ¨µÀ0ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P1        0x12  //½ÓÊÕÊý¾ÝÍ¨µÀ1ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P2        0x13  //½ÓÊÕÊý¾ÝÍ¨µÀ2ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P3        0x14  //½ÓÊÕÊý¾ÝÍ¨µÀ3ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P4        0x15  //½ÓÊÕÊý¾ÝÍ¨µÀ4ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P5        0x16  //½ÓÊÕÊý¾ÝÍ¨µÀ5ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define FIFO_STATUS     0x17  //FIFO×´Ì¬¼Ä´æÆ÷;bit0,RX FIFO¼Ä´æÆ÷¿Õ±êÖ¾;bit1,RX FIFOÂú±êÖ¾;bit2,3,±£Áô
                              //bit4,TX FIFO¿Õ±êÖ¾;bit5,TX FIFOÂú±êÖ¾;bit6,1,Ñ­»··¢ËÍÉÏÒ»Êý¾Ý°ü.0,²»Ñ­»·;
/**********************************************************************************************************/

//NRF2401Æ¬Ñ¡ÐÅºÅ
#define CE_L      HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10,GPIO_PIN_RESET)
#define CE_H      HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10,GPIO_PIN_SET)

//SPIÆ¬Ñ¡ÐÅºÅ	
#define CSN_L     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_13,GPIO_PIN_RESET)
#define CSN_H     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_13,GPIO_PIN_SET)

//SPIÊ±ÖÓ
#define 	SCK_L	HAL_GPIO_WritePin(GPIOC , GPIO_PIN_10,GPIO_PIN_RESET)
#define 	SCK_H	HAL_GPIO_WritePin(GPIOC , GPIO_PIN_10,GPIO_PIN_SET)

//SPIÊä³ö
#define 	MOSI_L	HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5,GPIO_PIN_RESET)
#define 	MOSI_H	HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5,GPIO_PIN_SET)

//SPIÊäÈë
#define 	READ_MISO	HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)
   
//IRQÖÐ¶Ï½Å
#define IRQ_L     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_12,GPIO_PIN_RESET)
#define IRQ_H     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_12,GPIO_PIN_SET)

#define READ_IRQ    HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)


//NRF24L01·¢ËÍ½ÓÊÕÊý¾Ý¿í¶È¶¨Òå
#define TX_ADR_WIDTH    5                               //5×Ö½ÚµÄµØÖ·¿í¶È
#define RX_ADR_WIDTH    5                               //5×Ö½ÚµÄµØÖ·¿í¶È
#define TX_PLOAD_WIDTH  32                              //20×Ö½ÚµÄÓÃ»§Êý¾Ý¿í¶È
#define RX_PLOAD_WIDTH  32                              //20×Ö½ÚµÄÓÃ»§Êý¾Ý¿í¶È
									   	   

void Init_NRF24L01(void);                                //NRF24l01³õÊ¼»¯
uint8_t SPI_ReadWriteByte(uint8_t TxData) ;						 //Ä£ÄâSPIÍ¨Ñ¶º¯Êý
void RX_Mode(void);                                      //ÅäÖÃÎª½ÓÊÕÄ£Ê½
void TX_Mode(void);                                      //ÅäÖÃÎª·¢ËÍÄ£Ê½
uint8_t NRF24L01_Write_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen); //Ð´Êý¾ÝÇø
uint8_t NRF24L01_Read_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen);  //¶ÁÊý¾ÝÇø		  
uint8_t NRF24L01_Read_Reg(uint8_t regaddr);		                 //¶Á¼Ä´æÆ÷
uint8_t NRF24L01_Write_Reg(uint8_t regaddr, uint8_t data);              //Ð´¼Ä´æÆ÷
uint8_t NRF24L01_Check(void);                                 //¼ì²éNRF24L01ÊÇ·ñÔÚÎ»
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);                         //·¢ËÍÒ»¸ö°üµÄÊý¾Ý
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);                         //½ÓÊÕÒ»¸ö°üµÄÊý¾Ý


#endif


#if 0
#ifndef __NRF24L01_H__
#define __NRF24L01_H__
	 
#include "main.h"


#define NRF_CE	GPIO_PIN_10
#define NRF_CE_HIGH	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET)
#define NRF_CE_LOW	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET)

#define NRF_CS	GPIO_PIN_13 
#define NRF_CS_HIGH	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)
#define NRF_CS_LOW	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)

#define NRF_IRQ	GPIO_PIN_12 
// #define NRF_IRQ_STATUS	(GPIO_ReadInputDataBit(GPIOB, NRF_IRQ))
#define NRF_IRQ_STATUS	(HAL_GPIO_ReadPin(GPIOB, NRF_IRQ))

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01¼Ä´æÆ÷²Ù×÷ÃüÁî
#define NRF_READ_REG    0x00  //¶ÁÅäÖÃ¼Ä´æÆ÷,µÍ5Î»Îª¼Ä´æÆ÷µØÖ·
#define NRF_WRITE_REG   0x20  //Ð´ÅäÖÃ¼Ä´æÆ÷,µÍ5Î»Îª¼Ä´æÆ÷µØÖ·
#define RD_RX_PLOAD     0x61  //¶ÁRXÓÐÐ§Êý¾Ý,1~32×Ö½Ú
#define WR_TX_PLOAD     0xA0  //Ð´TXÓÐÐ§Êý¾Ý,1~32×Ö½Ú
#define FLUSH_TX        0xE1  //Çå³ýTX FIFO¼Ä´æÆ÷.·¢ÉäÄ£Ê½ÏÂÓÃ
#define FLUSH_RX        0xE2  //Çå³ýRX FIFO¼Ä´æÆ÷.½ÓÊÕÄ£Ê½ÏÂÓÃ
#define REUSE_TX_PL     0xE3  //ÖØÐÂÊ¹ÓÃÉÏÒ»°üÊý¾Ý,CEÎª¸ß,Êý¾Ý°ü±»²»¶Ï·¢ËÍ.
#define NOP             0xFF  //¿Õ²Ù×÷,¿ÉÒÔÓÃÀ´¶Á×´Ì¬¼Ä´æÆ÷	 
//SPI(NRF24L01)¼Ä´æÆ÷µØÖ·
#define CONFIG          0x00  //ÅäÖÃ¼Ä´æÆ÷µØÖ·;bit0:1½ÓÊÕÄ£Ê½,0·¢ÉäÄ£Ê½;bit1:µçÑ¡Ôñ;bit2:CRCÄ£Ê½;bit3:CRCÊ¹ÄÜ;
                              //bit4:ÖÐ¶ÏMAX_RT(´ïµ½×î´óÖØ·¢´ÎÊýÖÐ¶Ï)Ê¹ÄÜ;bit5:ÖÐ¶ÏTX_DSÊ¹ÄÜ;bit6:ÖÐ¶ÏRX_DRÊ¹ÄÜ
#define EN_AA           0x01  //Ê¹ÄÜ×Ô¶¯Ó¦´ð¹¦ÄÜ  bit0~5,¶ÔÓ¦Í¨µÀ0~5
#define EN_RXADDR       0x02  //½ÓÊÕµØÖ·ÔÊÐí,bit0~5,¶ÔÓ¦Í¨µÀ0~5
#define SETUP_AW        0x03  //ÉèÖÃµØÖ·¿í¶È(ËùÓÐÊý¾ÝÍ¨µÀ):bit1,0:00,3×Ö½Ú;01,4×Ö½Ú;02,5×Ö½Ú;
#define SETUP_RETR      0x04  //½¨Á¢×Ô¶¯ÖØ·¢;bit3:0,×Ô¶¯ÖØ·¢¼ÆÊýÆ÷;bit7:4,×Ô¶¯ÖØ·¢ÑÓÊ± 250*x+86us
#define RF_CH           0x05  //RFÍ¨µÀ,bit6:0,¹¤×÷Í¨µÀÆµÂÊ;
#define RF_SETUP        0x06  //RF¼Ä´æÆ÷;bit3:´«ÊäËÙÂÊ(0:1Mbps,1:2Mbps);bit2:1,·¢Éä¹¦ÂÊ;bit0:µÍÔëÉù·Å´óÆ÷ÔöÒæ
#define STATUS          0x07  //×´Ì¬¼Ä´æÆ÷;bit0:TX FIFOÂú±êÖ¾;bit3:1,½ÓÊÕÊý¾ÝÍ¨µÀºÅ(×î´ó:6);bit4,´ïµ½×î¶à´ÎÖØ·¢
                              //bit5:Êý¾Ý·¢ËÍÍê³ÉÖÐ¶Ï;bit6:½ÓÊÕÊý¾ÝÖÐ¶Ï;
#define MAX_TX  		0x10  //´ïµ½×î´ó·¢ËÍ´ÎÊýÖÐ¶Ï
#define TX_OK   		0x20  //TX·¢ËÍÍê³ÉÖÐ¶Ï
#define RX_OK   		0x40  //½ÓÊÕµ½Êý¾ÝÖÐ¶Ï

#define OBSERVE_TX      0x08  //·¢ËÍ¼ì²â¼Ä´æÆ÷,bit7:4,Êý¾Ý°ü¶ªÊ§¼ÆÊýÆ÷;bit3:0,ÖØ·¢¼ÆÊýÆ÷
#define CD              0x09  //ÔØ²¨¼ì²â¼Ä´æÆ÷,bit0,ÔØ²¨¼ì²â;
#define RX_ADDR_P0      0x0A  //Êý¾ÝÍ¨µÀ0½ÓÊÕµØÖ·,×î´ó³¤¶È5¸ö×Ö½Ú,µÍ×Ö½ÚÔÚÇ°
#define RX_ADDR_P1      0x0B  //Êý¾ÝÍ¨µÀ1½ÓÊÕµØÖ·,×î´ó³¤¶È5¸ö×Ö½Ú,µÍ×Ö½ÚÔÚÇ°
#define RX_ADDR_P2      0x0C  //Êý¾ÝÍ¨µÀ2½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define RX_ADDR_P3      0x0D  //Êý¾ÝÍ¨µÀ3½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define RX_ADDR_P4      0x0E  //Êý¾ÝÍ¨µÀ4½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define RX_ADDR_P5      0x0F  //Êý¾ÝÍ¨µÀ5½ÓÊÕµØÖ·,×îµÍ×Ö½Ú¿ÉÉèÖÃ,¸ß×Ö½Ú,±ØÐëÍ¬RX_ADDR_P1[39:8]ÏàµÈ;
#define TX_ADDR         0x10  //·¢ËÍµØÖ·(µÍ×Ö½ÚÔÚÇ°),ShockBurstTMÄ£Ê½ÏÂ,RX_ADDR_P0Óë´ËµØÖ·ÏàµÈ
#define RX_PW_P0        0x11  //½ÓÊÕÊý¾ÝÍ¨µÀ0ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P1        0x12  //½ÓÊÕÊý¾ÝÍ¨µÀ1ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P2        0x13  //½ÓÊÕÊý¾ÝÍ¨µÀ2ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P3        0x14  //½ÓÊÕÊý¾ÝÍ¨µÀ3ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P4        0x15  //½ÓÊÕÊý¾ÝÍ¨µÀ4ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define RX_PW_P5        0x16  //½ÓÊÕÊý¾ÝÍ¨µÀ5ÓÐÐ§Êý¾Ý¿í¶È(1~32×Ö½Ú),ÉèÖÃÎª0Ôò·Ç·¨
#define NRF_FIFO_STATUS 0x17  //FIFO×´Ì¬¼Ä´æÆ÷;bit0,RX FIFO¼Ä´æÆ÷¿Õ±êÖ¾;bit1,RX FIFOÂú±êÖ¾;bit2,3,±£Áô
                              //bit4,TX FIFO¿Õ±êÖ¾;bit5,TX FIFOÂú±êÖ¾;bit6,1,Ñ­»··¢ËÍÉÏÒ»Êý¾Ý°ü.0,²»Ñ­»·;
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//nrf24l01 Send data width definition
#define TX_ADR_WIDTH		5	//5 byte address width
#define RX_ADR_WIDTH		5	//5 byte address width
#define TX_PLOAD_WIDTH		5//32	//32 bytes user data width
#define RX_PLOAD_WIDTH		5//32	//32 bytes user data width

#define MAX_TX				0x10	//Reached the maximum number of sending interrupts
#define TX_OK				0x20	//send complete interrupt
#define RX_OK				0x40	//receive complete interrupt

void NRF24L01_GPIO_Init(void);
uint8_t NRF24L01_Write_Reg(uint8_t res, uint8_t value);
uint8_t NRF24L01_Read_Reg(uint8_t res);
//uint8_t NRF24L01_Read_Buf(uint8_t res, uint8_t len, uint8_t *pBuf);

uint8_t NRF24L01_Read_Buf(uint8_t res, uint8_t *pBuf, uint8_t len);

uint8_t NRF24L01_Check(void);
void NRF24L01_Power_Down_Mode(void);
void NRF24L01_TX_Mode(void);
void NRF24L01_RX_Mode(void);
uint8_t NRF24L01_TX_Packet(uint8_t *txbuf);
uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf);
void NRF24L01_Init(void);

#endif


#endif





