/*
 * TIMER.hpp
 *
 *  Created on: Aug 31, 2021
 *      Author: Xunguo Zhu
 */
#ifndef _TIMER_H
#define _TIMER_H
#include "tim.h"

#ifdef __cplusplus

#define u8 unsigned char
#define u16 unsigned int

#define Timer_Mode_Timer 0
#define Timer_Mode_PWM 1
//#define Timer_Mode_ 2

#define Init_Success 1

class Timer{
	public:
		Timer(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u8 mode, u16 arr, u16 pcs);
		~Timer();
		void PWM_Out(u8 n, u16 pwm);
		void Timer_Interrupt();

	public:
		u8 fre_1000hz;
		u8 fre_500hz;
		u8 fre_200hz;
		u8 fre_100hz;
		u8 fre_10hz;
		u8 fre_1hz;

	private:
		u16 arr;
		u16 pcs;
		u16 pwm;
		TIM_HandleTypeDef *tim;

		u8 count_1ms;
		u8 count_2ms;
		u8 count_5ms;
		u8 count_10ms;
		u8 count_100ms;
		u16 count_1s;

	private:
		u8 Senior_Timer_PWM_Init();
		u8 Task_Scheduler_Init();

};

#endif

#endif
