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
		Timer(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs);
//	public:
//		u8 fre_1000hz;
//		u8 fre_500hz;
//		u8 fre_200hz;
//		u8 fre_100hz;
//		u8 fre_10hz;
//		u8 fre_1hz;

	protected:
		u16 arr;
		u16 pcs;
		TIM_HandleTypeDef *tim;
		TIM_TypeDef *Tim_Num;


};

class Timer_PWM : public Timer{
	public:
		Timer_PWM(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs);
		~Timer_PWM();
		void PWM_Out(u8 n, u16 pwm);

	private:
		u16 pwm;
};

#endif

#endif
