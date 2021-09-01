/*
 * Task_Scheduler.hpp
 *
 *  Created on: Sep 1, 2021
 *      Author: Xunguo Zhu
 */
#ifndef _TASK_SCHEDULER_H
#define _TASK_SCHEDULER_H

#include "TIMER.hpp"

class Task_Scheduler : public Timer{
	public:
		Task_Scheduler();
		~Task_Scheduler();
		void Timer_Interrupt();
		void Task();
		void Task_1000hz();
		void Task_500hz();
		void Task_200hz();
		void Task_100hz();
		void Task_10hz();
		void Task_1hz();

	public:
		u8 fre_1000hz;
		u8 fre_500hz;
		u8 fre_200hz;
		u8 fre_100hz;
		u8 fre_10hz;
		u8 fre_1hz;

	private:
		u8 count_1ms;
		u8 count_2ms;
		u8 count_5ms;
		u8 count_10ms;
		u8 count_100ms;
		u16 count_1s;

};

#endif
