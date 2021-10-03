#include "USART.hpp"

static void UART_SetConfig(UART_HandleTypeDef *huart);

Usart::Usart(UART_HandleTypeDef *usart, USART_TypeDef *usart_num, unsigned int baud)
{
	this->usart = usart;
	this->usart_num = usart_num;
	this->baud = baud;

	this->usart->Instance = this->usart_num;
	this->usart->Init.BaudRate = this->baud;
	this->usart->Init.WordLength = UART_WORDLENGTH_8B;
	this->usart->Init.StopBits = UART_STOPBITS_1;
	this->usart->Init.Parity = UART_PARITY_NONE;
	this->usart->Init.Mode = UART_MODE_TX_RX;
	this->usart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	this->usart->Init.OverSampling = UART_OVERSAMPLING_16;
	if (this->USART_Init() != HAL_OK)
	{
	Error_Handler();
	}
}

Usart::~Usart()
{
	this->USART_MspDeInit();
}

void Usart::Send_String(uint8_t *str)
{
	uint8_t k=0;
	do{
		HAL_UART_Transmit(this->usart, (uint8_t *)(str + k), 1, 1000);
		k++;
	}while(*(str + k)!='\0');

}

void Usart::USART_MspInit()
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(this->usart->Instance == USART1){
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* USART1 interrupt Init */
//		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}


void Usart::USART_MspDeInit()
{

	if(this->usart->Instance == USART1){
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

		/* USART1 interrupt Deinit */
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

HAL_StatusTypeDef Usart::USART_Init()
{
  /* Check the UART handle allocation */
	if (this->usart == NULL){
		return HAL_ERROR;
	}
	if (this->usart->Init.HwFlowCtl != UART_HWCONTROL_NONE){
		assert_param(IS_UART_HWFLOW_INSTANCE(this->usart->Instance));
		assert_param(IS_UART_HARDWARE_FLOW_CONTROL(this->usart->Init.HwFlowCtl));
	}
	else{
		assert_param(IS_UART_INSTANCE(this->usart->Instance));
	}
	assert_param(IS_UART_WORD_LENGTH(this->usart->Init.WordLength));
	assert_param(IS_UART_OVERSAMPLING(this->usart->Init.OverSampling));

	if (this->usart->gState == HAL_UART_STATE_RESET){
    this->usart->Lock = HAL_UNLOCKED;
    this->USART_MspInit();
	}

	this->usart->gState = HAL_UART_STATE_BUSY;

	__HAL_UART_DISABLE(this->usart);

	UART_SetConfig(this->usart);

	CLEAR_BIT(this->usart->Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
	CLEAR_BIT(this->usart->Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	__HAL_UART_ENABLE(this->usart);

	this->usart->ErrorCode = HAL_UART_ERROR_NONE;
	this->usart->gState = HAL_UART_STATE_READY;
	this->usart->RxState = HAL_UART_STATE_READY;

	return HAL_OK;
}

static void UART_SetConfig(UART_HandleTypeDef *huart)
{
  uint32_t tmpreg;
  uint32_t pclk;

  /* Check the parameters */
  assert_param(IS_UART_BAUDRATE(huart->Init.BaudRate));
  assert_param(IS_UART_STOPBITS(huart->Init.StopBits));
  assert_param(IS_UART_PARITY(huart->Init.Parity));
  assert_param(IS_UART_MODE(huart->Init.Mode));

  /*-------------------------- USART CR2 Configuration -----------------------*/
  /* Configure the UART Stop Bits: Set STOP[13:12] bits
     according to huart->Init.StopBits value */
  MODIFY_REG(huart->Instance->CR2, USART_CR2_STOP, huart->Init.StopBits);

  /*-------------------------- USART CR1 Configuration -----------------------*/
  /* Configure the UART Word Length, Parity and mode:
     Set the M bits according to huart->Init.WordLength value
     Set PCE and PS bits according to huart->Init.Parity value
     Set TE and RE bits according to huart->Init.Mode value
     Set OVER8 bit according to huart->Init.OverSampling value */

  tmpreg = (uint32_t)huart->Init.WordLength | huart->Init.Parity | huart->Init.Mode | huart->Init.OverSampling;
  MODIFY_REG(huart->Instance->CR1,
             (uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8),
             tmpreg);

  /*-------------------------- USART CR3 Configuration -----------------------*/
  /* Configure the UART HFC: Set CTSE and RTSE bits according to huart->Init.HwFlowCtl value */
  MODIFY_REG(huart->Instance->CR3, (USART_CR3_RTSE | USART_CR3_CTSE), huart->Init.HwFlowCtl);


#if defined(USART6) && defined(UART9) && defined(UART10)
    if ((huart->Instance == USART1) || (huart->Instance == USART6) || (huart->Instance == UART9) || (huart->Instance == UART10))
    {
      pclk = HAL_RCC_GetPCLK2Freq();
    }
#elif defined(USART6)
    if ((huart->Instance == USART1) || (huart->Instance == USART6))
    {
      pclk = HAL_RCC_GetPCLK2Freq();
    }
#else
    if (huart->Instance == USART1)
    {
      pclk = HAL_RCC_GetPCLK2Freq();
    }
#endif /* USART6 */
    else
    {
      pclk = HAL_RCC_GetPCLK1Freq();
    }
  /*-------------------------- USART BRR Configuration ---------------------*/
  if (huart->Init.OverSampling == UART_OVERSAMPLING_8)
  {
    huart->Instance->BRR = UART_BRR_SAMPLING8(pclk, huart->Init.BaudRate);
  }
  else
  {
    huart->Instance->BRR = UART_BRR_SAMPLING16(pclk, huart->Init.BaudRate);
  }
}
