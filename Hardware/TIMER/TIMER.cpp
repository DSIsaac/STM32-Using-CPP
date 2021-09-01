/*
 * TIMER.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: Xunguo Zhu
 */
#include "tim.h"
#include "TIMER.hpp"

Timer::Timer(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u8 mode, u16 arr, u16 pcs)
{
	this->count_1ms = 0;
	this->count_2ms = 0;
	this->count_5ms = 0;
	this->count_10ms = 0;
	this->count_100ms = 0;
	this->count_1s = 0;
	this->fre_1000hz = 0;
	this->fre_500hz = 0;
	this->fre_200hz = 0;
	this->fre_100hz = 0;
	this->fre_10hz = 0;
	this->fre_1hz = 0;

	this->arr = arr;
	this->pcs = pcs;
	this->tim = tim;
	this->tim->Instance = Tim_Num;

	if((Tim_Num == TIM1 || Tim_Num == TIM8) && (mode == Timer_Mode_PWM)){
		Senior_Timer_PWM_Init();
	}
	else if(Tim_Num == TIM6 && mode == Timer_Mode_Timer){
		Task_Scheduler_Init();
	}
}

u8 Timer::Senior_Timer_PWM_Init()
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
	  if (HAL_TIM_Base_Init(this->tim) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(this->tim, &sClockSourceConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_TIM_PWM_Init(this->tim) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(this->tim, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	  if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(this->tim, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	  sBreakDeadTimeConfig.DeadTime = 0;
	  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	  if (HAL_TIMEx_ConfigBreakDeadTime(this->tim, &sBreakDeadTimeConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  HAL_TIM_MspPostInit(this->tim);
	  HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_2);
	  HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_3);
	  HAL_TIM_PWM_Start(this->tim, TIM_CHANNEL_4);
	  return Init_Success;
}

void Timer::PWM_Out(u8 n, u16 pwm)
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

u8 Timer::Task_Scheduler_Init(void)
{
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
	return Init_Success;
}

void Timer::Timer_Interrupt(void)
{
	this->count_1ms++;
	this->count_2ms++;
	this->count_5ms++;
	this->count_10ms++;
	this->count_100ms++;
	this->count_1s++;
	if(this->count_1ms >= 1){
		this->fre_1000hz = 1;
		this->count_1ms = 0;
	}
	if(this->count_2ms >= 2){
		this->fre_500hz = 1;
		this->count_2ms = 0;
	}
	if(this->count_5ms >= 5){
		this->fre_200hz = 1;
		this->count_5ms = 0;
	}
	if(this->count_10ms >= 10){
		this->fre_100hz = 1;
		this->count_10ms = 0;
	}
	if(this->count_100ms >= 100){
		this->fre_10hz = 1;
		this->count_100ms = 0;
	}
	if(this->count_1s >= 1000){
		this->fre_1hz = 1;
		this->count_1s = 0;
	}

}


Timer::~Timer()
{
	delete this;
}
