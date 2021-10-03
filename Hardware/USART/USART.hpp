#ifndef USART_H
#define USART_H

#include "main.h"
#include <cstdio>

class Usart{
	public:
		Usart(UART_HandleTypeDef *usart, USART_TypeDef *usart_num, unsigned int baud);
		~Usart();
		void Send_String(uint8_t *str);

	public:
		UART_HandleTypeDef *usart;
		USART_TypeDef *usart_num;
		unsigned int baud;

	protected:
		void USART_MspInit();
		void USART_MspDeInit();
		HAL_StatusTypeDef USART_Init();
};

#endif
