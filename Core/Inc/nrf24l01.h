#ifndef _NRF24L01_H
#define _NRF24L01_H

#include "main.h"
/****************************************************************************************************/
//NRF24L01 commands
#define SPI_READ_REG    0x00
#define SPI_WRITE_REG   0x20
#define RD_RX_PLOAD     0x61
#define WR_TX_PLOAD     0xA0
#define FLUSH_TX        0xE1
#define FLUSH_RX        0xE2
#define REUSE_TX_PL     0xE3
#define NOP             0xFF
 
//register addres
#define CONFIG          0x00
                            
#define EN_AA           0x01
#define EN_RXADDR       0x02
#define SETUP_AW        0x03
#define SETUP_RETR      0x04
#define RF_CH           0x05
#define RF_SETUP        0x06
#define STATUS          0x07
                            
#define MAX_TX  	    0x10
#define TX_OK       	0x20
#define RX_OK   	    0x40

#define OBSERVE_TX      0x08
#define CD              0x09
#define RX_ADDR_P0      0x0A
#define RX_ADDR_P1      0x0B
#define RX_ADDR_P2      0x0C
#define RX_ADDR_P3      0x0D
#define RX_ADDR_P4      0x0E
#define RX_ADDR_P5      0x0F
#define TX_ADDR         0x10
#define RX_PW_P0        0x11
#define RX_PW_P1        0x12
#define RX_PW_P2        0x13
#define RX_PW_P3        0x14
#define RX_PW_P4        0x15
#define RX_PW_P5        0x16
#define FIFO_STATUS     0x17
/**********************************************************************************************************/

#define CE_L      HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10,GPIO_PIN_RESET)
#define CE_H      HAL_GPIO_WritePin(GPIOB ,GPIO_PIN_10,GPIO_PIN_SET)


#define CSN_L     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_13,GPIO_PIN_RESET)
#define CSN_H     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_13,GPIO_PIN_SET)


#define 	SCK_L	HAL_GPIO_WritePin(GPIOC , GPIO_PIN_10,GPIO_PIN_RESET)
#define 	SCK_H	HAL_GPIO_WritePin(GPIOC , GPIO_PIN_10,GPIO_PIN_SET)


#define 	MOSI_L	HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5,GPIO_PIN_RESET)
#define 	MOSI_H	HAL_GPIO_WritePin(GPIOB , GPIO_PIN_5,GPIO_PIN_SET)


#define 	READ_MISO	HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11)
   

#define IRQ_L     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_12,GPIO_PIN_RESET)
#define IRQ_H     HAL_GPIO_WritePin(GPIOB , GPIO_PIN_12,GPIO_PIN_SET)

#define READ_IRQ    HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)



#define TX_ADR_WIDTH    5
#define RX_ADR_WIDTH    5
#define TX_PLOAD_WIDTH  32
#define RX_PLOAD_WIDTH  32
									   	   

void Init_NRF24L01(void);
uint8_t SPI_ReadWriteByte(uint8_t TxData) ;
void RX_Mode(void);
void TX_Mode(void);
uint8_t NRF24L01_Write_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen);
uint8_t NRF24L01_Read_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen);
uint8_t NRF24L01_Read_Reg(uint8_t regaddr);
uint8_t NRF24L01_Write_Reg(uint8_t regaddr, uint8_t data);
uint8_t NRF24L01_Check(void);
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);


#endif

