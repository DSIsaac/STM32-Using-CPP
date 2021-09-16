/*
 * SPI.h
 *
 *  Created on: Sep 16, 2021
 *      Author: Xunguo Zhu
 */

#ifndef _SPI_H
#define _SPI_H

#include "main.h"

class SPI {
	public:
		SPI(SPI_HandleTypeDef *spi, SPI_TypeDef *spi_num);
		virtual ~SPI();


	public:
		SPI_HandleTypeDef *spi;
		SPI_TypeDef *spi_num;

	private:
		void SPI_MspInit(SPI_HandleTypeDef* spiHandle);
		void SPI_MspDeInit(SPI_HandleTypeDef* spiHandle);

};

#endif /* SPI_SPI_H_ */
