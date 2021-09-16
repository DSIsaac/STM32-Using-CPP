/*
 * SPI.cpp
 *
 *  Created on: Sep 16, 2021
 *      Author: HASEE
 */

#include "SPI.hpp"

SPI::SPI(SPI_HandleTypeDef *spi, SPI_TypeDef *spi_num)
{
	this->spi = spi;
	this->spi_num = spi_num;

	this->spi->Instance = this->spi_num;
	this->spi->Init.Mode = SPI_MODE_MASTER;
	this->spi->Init.Direction = SPI_DIRECTION_2LINES;
	this->spi->Init.DataSize = SPI_DATASIZE_8BIT;
	this->spi->Init.CLKPolarity = SPI_POLARITY_HIGH;
	this->spi->Init.CLKPhase = SPI_PHASE_2EDGE;
	this->spi->Init.NSS = SPI_NSS_SOFT;
	this->spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	this->spi->Init.FirstBit = SPI_FIRSTBIT_MSB;
	this->spi->Init.TIMode = SPI_TIMODE_DISABLE;
	this->spi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	this->spi->Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(this->spi) != HAL_OK){
		Error_Handler();
	}
}

SPI::~SPI()
{
	SPI_MspDeInit(this->spi);
}

void SPI::SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(spiHandle->Instance==SPI2){
		/* SPI2 clock enable */
		__HAL_RCC_SPI2_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**SPI2 GPIO Configuration
		PB13     ------> SPI2_SCK
		PB14     ------> SPI2_MISO
		PB15     ------> SPI2_MOSI
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

void SPI::SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

	if(spiHandle->Instance == SPI2){
	__HAL_RCC_SPI2_CLK_DISABLE();
	/**SPI2 GPIO Configuration
	PB13     ------> SPI2_SCK
	PB14     ------> SPI2_MISO
	PB15     ------> SPI2_MOSI
	*/
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	}
}

