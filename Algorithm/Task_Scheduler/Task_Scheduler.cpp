/*
 * Task_Scheduler.cpp
 *
 *  Created on: Sep 1, 2021
 *      Author: Xunguo Zhu
 */
#include "Task_Scheduler.hpp"

extern Timer_PWM *Timer1;
extern Timer_PWM *Timer8;

void Task_Scheduler::Task()
{
	if(this->fre_1000hz == 1){
		this->Task_1000hz();
		this->fre_1000hz = 0;
	}
	if(this->fre_500hz == 1){
		this->Task_500hz();
		this->fre_500hz = 0;
	}
	if(this->fre_200hz == 1){
		this->Task_200hz();
		this->fre_200hz = 0;
	}
	if(this->fre_100hz == 1){
		this->Task_100hz();
		this->fre_100hz = 0;
	}
	if(this->fre_10hz == 1){
		this->Task_10hz();
		this->fre_10hz = 0;
	}
	if(this->fre_1hz == 1){
		this->Task_1hz();
		this->fre_1hz = 0;
	}
}
void Task_Scheduler::Task_1000hz()
{
	Timer1->PWM_Out(1, 100);
}
void Task_Scheduler::Task_500hz()
{
	Timer1->PWM_Out(2, 200);
}
void Task_Scheduler::Task_200hz()
{
	Timer1->PWM_Out(3, 300);
}
void Task_Scheduler::Task_100hz()
{
	Timer1->PWM_Out(4, 400);
}
void Task_Scheduler::Task_10hz()
{
	Timer8->PWM_Out(1, 500);
}
void Task_Scheduler::Task_1hz()
{
	Timer8->PWM_Out(2, 600);
}

Task_Scheduler::Task_Scheduler(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs):Timer(tim, Tim_Num, arr, pcs)
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	this->tim->Init.Prescaler = this->pcs;
	this->tim->Init.CounterMode = TIM_COUNTERMODE_UP;
	this->tim->Init.Period = this->arr;
	this->tim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(this->tim) != HAL_OK){
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK){
		Error_Handler();
	}
	HAL_TIM_Base_Start_IT(this->tim);
}

void Task_Scheduler::Timer_Interrupt()
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

Task_Scheduler::~Task_Scheduler()
{

}
