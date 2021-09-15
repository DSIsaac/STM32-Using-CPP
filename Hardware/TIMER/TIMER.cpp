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
	HAL_TIM_Base_MspDeInit(this->tim);
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

 }

 Timer_Input_Capture::~Timer_Input_Capture()
 {
	 HAL_TIM_Base_MspDeInit(this->tim);
 }
