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
	if (this->SPI_Init() != HAL_OK){
		Error_Handler();
	}
}

SPI::~SPI()
{
	SPI_MspDeInit(this->spi);
}

HAL_StatusTypeDef SPI::SPI_Init()
{
  /* Check the SPI handle allocation */
	if (this->spi == NULL){
		return HAL_ERROR;
	}
  /* Check the parameters */
	assert_param(IS_SPI_ALL_INSTANCE(this->spi->Instance));
	assert_param(IS_SPI_MODE(this->spi->Init.Mode));
	assert_param(IS_SPI_DIRECTION(this->spi->Init.Direction));
	assert_param(IS_SPI_DATASIZE(this->spi->Init.DataSize));
	assert_param(IS_SPI_NSS(this->spi->Init.NSS));
	assert_param(IS_SPI_BAUDRATE_PRESCALER(this->spi->Init.BaudRatePrescaler));
	assert_param(IS_SPI_FIRST_BIT(this->spi->Init.FirstBit));
	assert_param(IS_SPI_TIMODE(this->spi->Init.TIMode));
	if (this->spi->Init.TIMode == SPI_TIMODE_DISABLE){
		assert_param(IS_SPI_CPOL(this->spi->Init.CLKPolarity));
		assert_param(IS_SPI_CPHA(this->spi->Init.CLKPhase));

		if (this->spi->Init.Mode == SPI_MODE_MASTER){
			assert_param(IS_SPI_BAUDRATE_PRESCALER(this->spi->Init.BaudRatePrescaler));
		}
		else{
		  /* Baudrate prescaler not use in Motoraola Slave mode. force to default value */
		  this->spi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
		}
	}
	else{
		assert_param(IS_SPI_BAUDRATE_PRESCALER(this->spi->Init.BaudRatePrescaler));
		/* Force polarity and phase to TI protocaol requirements */
		this->spi->Init.CLKPolarity = SPI_POLARITY_LOW;
		this->spi->Init.CLKPhase    = SPI_PHASE_1EDGE;
	}
#if (USE_SPI_CRC != 0U)
	assert_param(IS_SPI_CRC_CALCULATION(this->spi->Init.CRCCalculation));
	if (this->spi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE){
		assert_param(IS_SPI_CRC_POLYNOMIAL(this->spi->Init.CRCPolynomial));
	}
#else
	this->spi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
#endif /* USE_SPI_CRC */

	if (this->spi->State == HAL_SPI_STATE_RESET){
	/* Allocate lock resource and initialize it */
		this->spi->Lock = HAL_UNLOCKED;
		this->SPI_MspInit(this->spi);
	}

	this->spi->State = HAL_SPI_STATE_BUSY;

  /* Disable the selected SPI peripheral */
	__HAL_SPI_DISABLE(this->spi);

	/*----------------------- SPIx CR1 & CR2 Configuration ---------------------*/
	/* Configure : SPI Mode, Communication Mode, Data size, Clock polarity and phase, NSS management,
	Communication speed, First bit and CRC calculation state */
	WRITE_REG(this->spi->Instance->CR1, ((this->spi->Init.Mode & (SPI_CR1_MSTR | SPI_CR1_SSI)) |
								  (this->spi->Init.Direction & (SPI_CR1_RXONLY | SPI_CR1_BIDIMODE)) |
								  (this->spi->Init.DataSize & SPI_CR1_DFF) |
								  (this->spi->Init.CLKPolarity & SPI_CR1_CPOL) |
								  (this->spi->Init.CLKPhase & SPI_CR1_CPHA) |
								  (this->spi->Init.NSS & SPI_CR1_SSM) |
								  (this->spi->Init.BaudRatePrescaler & SPI_CR1_BR_Msk) |
								  (this->spi->Init.FirstBit  & SPI_CR1_LSBFIRST) |
								  (this->spi->Init.CRCCalculation & SPI_CR1_CRCEN)));

	/* Configure : NSS management, TI Mode */
	WRITE_REG(this->spi->Instance->CR2, (((this->spi->Init.NSS >> 16U) & SPI_CR2_SSOE) | (this->spi->Init.TIMode & SPI_CR2_FRF)));

#if (USE_SPI_CRC != 0U)
  /*---------------------------- SPIx CRCPOLY Configuration ------------------*/
  /* Configure : CRC Polynomial */
	if (this->spi->Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE){
		WRITE_REG(this->spi->Instance->CRCPR, (this->spi->Init.CRCPolynomial & SPI_CRCPR_CRCPOLY_Msk));
	}
#endif /* USE_SPI_CRC */

#if defined(SPI_I2SCFGR_I2SMOD)
  /* Activate the SPI mode (Make sure that I2SMOD bit in I2SCFGR register is reset) */
	CLEAR_BIT(this->spi->Instance->I2SCFGR, SPI_I2SCFGR_I2SMOD);
#endif /* SPI_I2SCFGR_I2SMOD */

	this->spi->ErrorCode = HAL_SPI_ERROR_NONE;
	this->spi->State     = HAL_SPI_STATE_READY;

	return HAL_OK;
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

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

		/*Configure GPIO pin : PA8 */
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
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
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
	}
}

