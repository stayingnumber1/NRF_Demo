#include "nrf24l01.h"
#include "spi.h"
#include "usart.h"

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; // send model address
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; // receive model address

// 1.Use software to simulate the SPI bus,do not use the SPI with the STM32G4.
// 2.and the SPI of the G4 has not been debugged yet.
// 3.Using the SPI that comes with the STM can complete chip identification but cannot transmit data.

/*                       NRF24L01 IO Map
  ----------------------------------------------------------------
  CE      PB10
  CSN     PB13
  SCK     PC10
  MOSI    PB5
  MISO    PC11
  IRQ     PB12
----------------------------------------------------------------*/

/**
 * @brief
 */
void Init_NRF24L01(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

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

	CE_H;
	CSN_H;

	CE_L;
	CSN_H;
}

/**
 * @brief core bus 160M
 */
void SOFT_SPI_Delay(uint8_t us)
{
	for (int i = 0; i < us; i++)
	{
		__asm("NOP"); // core bus 160M  Çé¿öÏÂ´ó¸ÅIICËÙÂÊ 400K
	}
}

/**
 * @brief　Simulate SPI using software instead of hardware
 */
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
	uint16_t bit_ctr;
	for (bit_ctr = 0; bit_ctr < 8; bit_ctr++)
	{
		if (TxData & 0x80)
			MOSI_H;
		else
			MOSI_L;
		TxData = (TxData << 1);
		SCK_H;
		SOFT_SPI_Delay(10);
		if (READ_MISO)
			TxData |= 0x01;
		SCK_L;
		SOFT_SPI_Delay(10);
	}
	return (TxData);
}

uint8_t NRF24L01_Write_Reg(uint8_t reg_addr, uint8_t data)
{
	uint8_t status;
	CSN_L;
	status = SPI_ReadWriteByte(reg_addr);
	SPI_ReadWriteByte(data);
	CSN_H;
	return (status);
}

uint8_t NRF24L01_Read_Reg(uint8_t reg_addr)
{
	uint8_t reg_val;
	CSN_L;
	SPI_ReadWriteByte(reg_addr);
	reg_val = SPI_ReadWriteByte(0);

	CSN_H;
	return (reg_val);
}

uint8_t NRF24L01_Read_Buf(uint8_t reg_addr, uint8_t *pBuf, uint8_t data_len)
{
	uint8_t status, i;
	CSN_L;
	status = SPI_ReadWriteByte(reg_addr);
	for (i = 0; i < data_len; i++)
		pBuf[i] = SPI_ReadWriteByte(0);

	CSN_H;
	return status;
}

uint8_t NRF24L01_Write_Buf(uint8_t reg_addr, uint8_t *pBuf, uint8_t data_len)
{
	uint8_t status, i;
	CSN_L;
	status = SPI_ReadWriteByte(reg_addr);
	for (i = 0; i < data_len; i++)
		SPI_ReadWriteByte(*pBuf++);
	CSN_H;
	return status;
}

uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	uint8_t buf1[5];
	uint8_t i;
	NRF24L01_Write_Buf(SPI_WRITE_REG + TX_ADDR, buf, 5);
	NRF24L01_Read_Buf(TX_ADDR, buf1, 5);
	for (i = 0; i < 5; i++)
		if (buf1[i] != 0XA5)
			break;
	if (i != 5)
		return 1;
	return 0;
}

uint8_t NRF24L01_TxPacket(uint8_t *tx_buf)
{
	uint8_t state;
	CE_L;
	NRF24L01_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);
	CE_H;
	while (READ_IRQ != 0)
		;
	state = NRF24L01_Read_Reg(STATUS);
	NRF24L01_Write_Reg(SPI_WRITE_REG + STATUS, state);

	if (state & MAX_TX)
	{
		NRF24L01_Write_Reg(FLUSH_TX, 0xff);
		return MAX_TX;
	}
	if (state & TX_OK)
	{
		return TX_OK;
	}

	return 0xff;
}

uint8_t NRF24L01_RxPacket(uint8_t *rx_buf)
{
	uint8_t state;
	state = NRF24L01_Read_Reg(STATUS);
	NRF24L01_Write_Reg(SPI_WRITE_REG + STATUS, state);
	if (state & RX_OK)
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD, rx_buf, RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);
		return 0;
	}
	return 1;
}

void RX_Mode(void)
{
	CE_L;
	NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH);
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, 40);
	NRF24L01_Write_Reg(SPI_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);
	NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0f);
	CE_H;
}

void TX_Mode(void)
{
	CE_L;
	NRF24L01_Write_Buf(SPI_WRITE_REG + TX_ADDR, (uint8_t *)TX_ADDRESS, TX_ADR_WIDTH);
	NRF24L01_Write_Buf(SPI_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH);
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_AA, 0x01);
	NRF24L01_Write_Reg(SPI_WRITE_REG + EN_RXADDR, 0x01);
	NRF24L01_Write_Reg(SPI_WRITE_REG + SETUP_RETR, 0x1a);
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_CH, 40);
	NRF24L01_Write_Reg(SPI_WRITE_REG + RF_SETUP, 0x0f);
	NRF24L01_Write_Reg(SPI_WRITE_REG + CONFIG, 0x0e);
	CE_H;
}