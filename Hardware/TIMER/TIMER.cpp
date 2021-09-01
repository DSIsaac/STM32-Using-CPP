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
//	this->count_1ms = 0;
//	this->count_2ms = 0;
//	this->count_5ms = 0;
//	this->count_10ms = 0;
//	this->count_100ms = 0;
//	this->count_1s = 0;
//	this->fre_1000hz = 0;
//	this->fre_500hz = 0;
//	this->fre_200hz = 0;
//	this->fre_100hz = 0;
//	this->fre_10hz = 0;
//	this->fre_1hz = 0;

	this->arr = arr;
	this->pcs = pcs;
	this->tim = tim;
	this->tim->Instance = Tim_Num;
}

Timer_PWM::Timer_PWM():Timer(tim, Tim_Num, arr, pcs)
{
	this->arr = arr;
	this->pcs = pcs;
	this->tim = tim;
	this->tim->Instance = Tim_Num;

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	this->tim->Init.Prescaler = this->pcs;
	this->tim->Init.CounterMode = TIM_COUNTERMODE_UP;
	this->tim->Init.Period = this->arr;
	this->tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(this->tim) != HAL_OK)
	{
	Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	{
	Error_Handler();
	}
	HAL_TIM_Base_Start_IT(this->tim);
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
