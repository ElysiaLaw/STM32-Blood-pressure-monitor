#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"
/********************************************
    注意：
		TIM1空闲
		TIM2被onenet心跳包占用，在命令启动时做中断循环，每30秒一循环
		TIM3被AHT20占用，在命令启动时做中断延迟，间隔80毫秒
        ************************************/


void Timer_Init(void);
void TIM_Start(TIM_TypeDef* TIMx);
void TIM_End(TIM_TypeDef* TIMx);

#endif
