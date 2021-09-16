/*
 * TIMER.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: Xunguo Zhu
 */
#include "TIMER.hpp"

Timer::Timer(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs)
{
	this->arr = arr;
	this->pcs = pcs;
	this->tim = tim;
	this->tim->Instance = Tim_Num;
}

Timer::~Timer()
{
	TIM_Base_MspDeInit(this->tim);
}

HAL_StatusTypeDef Timer::TIM_Base_Init()
{
	/* Check the TIM handle allocation */
	if (this->tim == NULL){
		return HAL_ERROR;
	}
	  /* Check the parameters */
	assert_param(IS_TIM_INSTANCE(this->tim->Instance));
	assert_param(IS_TIM_COUNTER_MODE(this->tim->Init.CounterMode));
	assert_param(IS_TIM_CLOCKDIVISION_DIV(this->tim->Init.ClockDivision));
	assert_param(IS_TIM_AUTORELOAD_PRELOAD(this->tim->Init.AutoReloadPreload));

	if (this->tim->State == HAL_TIM_STATE_RESET){
		/* Allocate lock resource and initialize it */
		this->tim->Lock = HAL_UNLOCKED;
		this->TIM_Base_MspInit(this->tim);
	}
	/* Set the TIM state */
	this->tim->State = HAL_TIM_STATE_BUSY;
	/* Set the Time Base configuration */
	TIM_Base_SetConfig(this->tim->Instance, &this->tim->Init);
	/* Initialize the DMA burst operation state */
	this->tim->DMABurstState = HAL_DMA_BURST_STATE_READY;
	/* Initialize the TIM channels state */
	TIM_CHANNEL_STATE_SET_ALL(this->tim, HAL_TIM_CHANNEL_STATE_READY);
	TIM_CHANNEL_N_STATE_SET_ALL(this->tim, HAL_TIM_CHANNEL_STATE_READY);
	/* Initialize the TIM state*/
	this->tim->State = HAL_TIM_STATE_READY;
	return HAL_OK;
}

void Timer::TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(tim_baseHandle->Instance==TIM1){
		__HAL_RCC_TIM1_CLK_ENABLE();
	}
	else if(tim_baseHandle->Instance == TIM2){
		/* TIM2 clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**TIM2 GPIO Configuration
		PA0-WKUP     ------> TIM2_CH1
		PA1     ------> TIM2_CH2
		PA2     ------> TIM2_CH3
		PA3     ------> TIM2_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	}
	else if(tim_baseHandle->Instance == TIM3){
		/* TIM3 clock enable */
		__HAL_RCC_TIM3_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**TIM3 GPIO Configuration
		PA6     ------> TIM3_CH1
		PA7     ------> TIM3_CH2
		PB0     ------> TIM3_CH3
		PB1     ------> TIM3_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if(tim_baseHandle->Instance == TIM4){
		/* TIM4 clock enable */
		__HAL_RCC_TIM4_CLK_ENABLE();

		__HAL_RCC_GPIOD_CLK_ENABLE();
		/**TIM4 GPIO Configuration
		PD12     ------> TIM4_CH1
		PD13     ------> TIM4_CH2
		PD14     ------> TIM4_CH3
		PD15     ------> TIM4_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
	else if(tim_baseHandle->Instance == TIM6){
		/* TIM6 clock enable */
		__HAL_RCC_TIM6_CLK_ENABLE();

		/* TIM6 interrupt Init */
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}
	else if(tim_baseHandle->Instance == TIM8){
		/* TIM8 clock enable */
		__HAL_RCC_TIM8_CLK_ENABLE();
	}
}

void Timer::TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(timHandle->Instance==TIM1){
		__HAL_RCC_GPIOE_CLK_ENABLE();
		/**TIM1 GPIO Configuration
		PE9     ------> TIM1_CH1
		PE11     ------> TIM1_CH2
		PE13     ------> TIM1_CH3
		PE14     ------> TIM1_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	}
	else if(timHandle->Instance==TIM8){
		__HAL_RCC_GPIOC_CLK_ENABLE();
		/**TIM8 GPIO Configuration
		PC6     ------> TIM8_CH1
		PC7     ------> TIM8_CH2
		PC8     ------> TIM8_CH3
		PC9     ------> TIM8_CH4
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}

}

void Timer::TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

	if(tim_baseHandle->Instance==TIM1){
		/* Peripheral clock disable */
		__HAL_RCC_TIM1_CLK_DISABLE();
	}
	else if(tim_baseHandle->Instance==TIM2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();

		/**TIM2 GPIO Configuration
		PA0-WKUP     ------> TIM2_CH1
		PA1     ------> TIM2_CH2
		PA2     ------> TIM2_CH3
		PA3     ------> TIM2_CH4
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	}
	else if(tim_baseHandle->Instance==TIM3)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM3_CLK_DISABLE();
		/**TIM3 GPIO Configuration
		PA6     ------> TIM3_CH1
		PA7     ------> TIM3_CH2
		PB0     ------> TIM3_CH3
		PB1     ------> TIM3_CH4
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6|GPIO_PIN_7);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0|GPIO_PIN_1);
	}
	else if(tim_baseHandle->Instance==TIM4){
		/* Peripheral clock disable */
		__HAL_RCC_TIM4_CLK_DISABLE();

		/**TIM4 GPIO Configuration
		PD12     ------> TIM4_CH1
		PD13     ------> TIM4_CH2
		PD14     ------> TIM4_CH3
		PD15     ------> TIM4_CH4
		*/
		HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
	}
	else if(tim_baseHandle->Instance==TIM6){
		/* Peripheral clock disable */
		__HAL_RCC_TIM6_CLK_DISABLE();
		/* TIM6 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
	}
	else if(tim_baseHandle->Instance==TIM8){
		/* Peripheral clock disable */
		__HAL_RCC_TIM8_CLK_DISABLE();
	}
}

Timer_PWM::Timer_PWM(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs):Timer(tim, Tim_Num, arr, pcs)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	this->tim->Init.Prescaler = this->pcs;
	this->tim->Init.CounterMode = TIM_COUNTERMODE_UP;
	this->tim->Init.Period = this->arr;
	this->tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	this->tim->Init.RepetitionCounter = 0;
	this->tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (this->TIM_Base_Init() != HAL_OK){//删掉tim.c后可能会出现问题
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(this->tim, &sClockSourceConfig) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(this->tim) != HAL_OK){
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(this->tim, &sMasterConfig) != HAL_OK){
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_2) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_3) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_4) != HAL_OK){
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(this->tim, &sBreakDeadTimeConfig) != HAL_OK){
		Error_Handler();
	}
	this->TIM_MspPostInit(this->tim);
	HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_4);
}

Timer_PWM::~Timer_PWM()
{

}

void Timer_PWM::PWM_Out(u8 n, u16 pwm)
{
	this->pwm = pwm;
	switch(n){
	case 1:
		__HAL_TIM_SET_COMPARE(this->tim, TIM_CHANNEL_1, this->pwm);
		break;
	case 2:
		__HAL_TIM_SET_COMPARE(this->tim, TIM_CHANNEL_2, this->pwm);
		break;
	case 3:
		__HAL_TIM_SET_COMPARE(this->tim, TIM_CHANNEL_3, this->pwm);
		break;
	case 4:
		__HAL_TIM_SET_COMPARE(this->tim, TIM_CHANNEL_4, this->pwm);
		break;
	}
}

Timer_Input_Capture::Timer_Input_Capture(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs):Timer(tim, Tim_Num, arr, pcs)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};
	this->tim->Init.Prescaler = this->pcs;
	this->tim->Init.CounterMode = TIM_COUNTERMODE_UP;
	this->tim->Init.Period = this->arr;
	this->tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	this->tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (this->TIM_Base_Init() != HAL_OK){
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(this->tim, &sClockSourceConfig) != HAL_OK){
	    Error_Handler();
	}
	if (HAL_TIM_IC_Init(this->tim) != HAL_OK){
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(this->tim, &sMasterConfig) != HAL_OK){
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_2) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_3) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_4) != HAL_OK){
		Error_Handler();
	}
	this->TIM_Base_MspInit(this->tim);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_2);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_3);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_4);
}

Timer_Input_Capture::Timer_Input_Capture(unsigned char mode, TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs):Timer(tim, Tim_Num, arr, pcs)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

	this->tim->Init.Prescaler = this->pcs;
	this->tim->Init.CounterMode = TIM_COUNTERMODE_UP;
	this->tim->Init.Period = this->arr;
	this->tim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	this->tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (this->TIM_Base_Init() != HAL_OK){
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(this->tim, &sClockSourceConfig) != HAL_OK){
	    Error_Handler();
	}
	if (HAL_TIM_IC_Init(this->tim) != HAL_OK){
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(this->tim, &sMasterConfig) != HAL_OK){
		Error_Handler();
	}
	if(mode == IC_Rise){
		sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	}
	else if(mode == IC_Fall){
		sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
	}
	else{
		Error_Handler();
	}
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_2) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_3) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_IC_ConfigChannel(this->tim, &sConfigIC, TIM_CHANNEL_4) != HAL_OK){
		Error_Handler();
	}
	HAL_TIM_Base_MspInit(this->tim);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_2);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_3);
	HAL_TIM_IC_Start(this->tim, TIM_CHANNEL_4);
}

Timer_Input_Capture::~Timer_Input_Capture()
{

}

void Timer_Input_Capture::Read_Value(void)
{
	this->IC_Value[0] = HAL_TIM_ReadCapturedValue(this->tim, TIM_CHANNEL_1);
	this->IC_Value[1] = HAL_TIM_ReadCapturedValue(this->tim, TIM_CHANNEL_2);
	this->IC_Value[2] = HAL_TIM_ReadCapturedValue(this->tim, TIM_CHANNEL_3);
	this->IC_Value[3] = HAL_TIM_ReadCapturedValue(this->tim, TIM_CHANNEL_4);

}

void Timer_Input_Capture::Read_Normalization_Value(unsigned char fre)
{
	this->Read_Value();
	this->IC_Normalization_Value[0] = this->IC_Value[0] * fre;
	this->IC_Normalization_Value[1] = this->IC_Value[1] * fre;
	this->IC_Normalization_Value[2] = this->IC_Value[2] * fre;
	this->IC_Normalization_Value[3] = this->IC_Value[3] * fre;
}
