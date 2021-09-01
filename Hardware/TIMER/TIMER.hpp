/*
 * TIMER.hpp
 *
 *  Created on: Aug 31, 2021
 *      Author: HASEE
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
		Timer(u8 n, u8 mode, u16 arr, u16 pcs);
		~Timer();
//		u8 Timer_Init(u8 n, u16 arr, u16 pcs);
		void PWM_Out(u8 n, u16 pwm);

	private:
		u16 arr;
		u16 pcs;
		u16 pwm;
		u8 num;
		TIM_HandleTypeDef *tim;

	private:
		u8 Senior_Timer_PWM_Init();
};

#endif

#endif
