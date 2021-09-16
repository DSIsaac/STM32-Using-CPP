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
#define IC_Rise 0
#define IC_Fall 1
//#define Timer_Mode_ 2

#define Init_Success 1

class Timer{
	public:
		Timer(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs);
		virtual ~Timer();

	protected:
		u16 arr;
		u16 pcs;
		TIM_HandleTypeDef *tim;
		TIM_TypeDef *Tim_Num;

	protected:
		virtual HAL_StatusTypeDef TIM_Base_Init();
		virtual void TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle);
		virtual void TIM_MspPostInit(TIM_HandleTypeDef* timHandle);
		virtual void TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle);
};

class Timer_PWM : public Timer{
	public:
		Timer_PWM(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs);
		~Timer_PWM();
		void PWM_Out(u8 n, u16 pwm);

	private:
		u16 pwm;
};

class Timer_Input_Capture : public Timer{
	public:
		Timer_Input_Capture(TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs);
		Timer_Input_Capture(unsigned char mode, TIM_HandleTypeDef *tim, TIM_TypeDef *Tim_Num, u16 arr, u16 pcs);
		~Timer_Input_Capture();
		void Read_Value();
		void Read_Normalization_Value(unsigned char fre);
		void Read_Normalization_Value(unsigned int fre);
//		void Read_Speed();

	public:
		unsigned char IC_Value[4];//
		unsigned int IC_Normalization_Value[4];
//		unsigned int Speed[4];
	private:


};

#endif

#endif
