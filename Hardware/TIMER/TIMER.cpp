/*
 * TIMER.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: Xunguo Zhu
 */
#include "tim.h"
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
	HAL_TIM_Base_MspDeInit(this->tim);
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
	if (HAL_TIM_Base_Init(this->tim) != HAL_OK){
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
	HAL_TIM_MspPostInit(this->tim);
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
	if (HAL_TIM_Base_Init(this->tim) != HAL_OK){
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
	HAL_TIM_Base_MspInit(this->tim);
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
	if (HAL_TIM_Base_Init(this->tim) != HAL_OK){
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
