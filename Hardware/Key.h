#ifndef __KEY_H
#define __KEY_H

#include "main.h"
/*********************************************

    注：为方便模块化拆解
        中断函数内部调用按键函数
        即执行函数与中断函数分离的
        
*********************************************/

/*******************************************
    按键全局变量 
        state为按键选择，从1开始至页面确定值
        con为案件确定，1为确认按下
        ************************************/
extern uint8_t Key_state;
extern uint8_t Key_con;//key.c


/********************************************
    按键检测启用全局变量
        变量为1时按键可用，为0时变量被禁用
        ************************************/
extern uint8_t Key_yes;//key.c

//定义按键引脚及其时钟
#define GPIOKEY1 GPIOB
#define GPIOKEY2 GPIOA
#define GPIOKEY3 GPIOA

#define PINKEY1 GPIO_Pin_0
#define PINKEY2 GPIO_Pin_6
#define PINKEY3 GPIO_Pin_4

#define RCCKEY1 RCC_APB2Periph_GPIOB
#define RCCKEY2 RCC_APB2Periph_GPIOA
#define RCCKEY3 RCC_APB2Periph_GPIOA


//定义按键上下拉电阻    下拉GPIO_Mode_IPD    上拉GPIO_Mode_IPU     浮空GPIO_Mode_IN_FLOATING

#define GPIO_ModeKEY1 GPIO_Mode_IPD
#define GPIO_ModeKEY2 GPIO_Mode_IPD
#define GPIO_ModeKEY3 GPIO_Mode_IPD

//定义NVIC中断
#define NVICKEY1 EXTI0_IRQn
#define NVICKEY2 EXTI9_5_IRQn
#define NVICKEY3 EXTI4_IRQn

//定义EXTI中断相关参数
#define GPIO_PortKEY1 GPIO_PortSourceGPIOB
#define GPIO_PortKEY2 GPIO_PortSourceGPIOA
#define GPIO_PortKEY3 GPIO_PortSourceGPIOA

#define GPIO_PinKEY1 GPIO_PinSource0
#define GPIO_PinKEY2 GPIO_PinSource6
#define GPIO_PinKEY3 GPIO_PinSource4

#define EXTI_LineKEY1 EXTI_Line0
#define EXTI_LineKEY2 EXTI_Line6
#define EXTI_LineKEY3 EXTI_Line4

//定义EXTI中断上升沿下降沿触发方式  EXTI_Trigger_Rising  EXTI_Trigger_Falling  EXTI_Trigger_Rising_Falling
#define EXTI_TriggerKEY1 EXTI_Trigger_Rising
#define EXTI_TriggerKEY2 EXTI_Trigger_Rising
#define EXTI_TriggerKEY3 EXTI_Trigger_Rising


void Key_Init(void);
void Key1_Exit(void);
void Key2_Exit(void);
void Key3_Exit(void);


/*********************中断函数示例****************************

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_LineKEY1) == SET)		//判断是否是外部中断线触发的中断
	{
        uint32_t i = 2000000;
        while (i)
            i -= 1;									//去抖
        
        
		EXTI_ClearITPendingBit(EXTI_LineKEY1);		//清除外部中断线的中断标志位
	}
}


**********************************************************/

#endif
