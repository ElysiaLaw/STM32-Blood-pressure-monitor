#include "key.h"

uint8_t Key_yes = 1;

/**
  * 函    数：按键初始化
  * 参    数：无
  * 返 回 值：无
  */
void Key_Init(void)
{
	/*            开启时钟               */
	RCC_APB2PeriphClockCmd(RCCKEY1, ENABLE);
    RCC_APB2PeriphClockCmd(RCCKEY2, ENABLE);
    RCC_APB2PeriphClockCmd(RCCKEY3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
	
    
    
	/*            GPIO初始化            */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_ModeKEY1;
	GPIO_InitStructure.GPIO_Pin = PINKEY1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOKEY1, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_ModeKEY2;
	GPIO_InitStructure.GPIO_Pin = PINKEY2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOKEY2, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_ModeKEY3;
	GPIO_InitStructure.GPIO_Pin = PINKEY3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOKEY2, &GPIO_InitStructure);
    
    //将外部中断线映射到GPIO，即选择外部中断引脚
    GPIO_EXTILineConfig(GPIO_PortKEY1,GPIO_PinKEY1);
    GPIO_EXTILineConfig(GPIO_PortKEY2,GPIO_PinKEY2);
    GPIO_EXTILineConfig(GPIO_PortKEY3,GPIO_PinKEY3);
    
    /*            EXIT配置            */

    EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = EXTI_LineKEY1;					//选择配置外部中断的14号线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_TriggerKEY1;		//指定外部中断线为下降沿触发
	EXTI_Init(&EXTI_InitStructure);
    

	EXTI_InitStructure.EXTI_Line = EXTI_LineKEY2;
    EXTI_InitStructure.EXTI_Trigger = EXTI_TriggerKEY2;
    EXTI_Init(&EXTI_InitStructure);
    

    EXTI_InitStructure.EXTI_Line = EXTI_LineKEY3;
    EXTI_InitStructure.EXTI_Trigger = EXTI_TriggerKEY3;
    EXTI_Init(&EXTI_InitStructure);
    
    
    /*            NVIC配置            */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = NVICKEY1;				//选择配置NVIC的TIM2线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
    
    NVIC_InitStructure.NVIC_IRQChannel = NVICKEY2;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = NVICKEY3;
    NVIC_Init(&NVIC_InitStructure);
	
}

void Key1_Exit(void)
{
    if (Key_yes == 1)
	{
        Key_state = Key_state - 1;
	}
}

void Key2_Exit(void)
{
    if (Key_yes == 1)
	{
        Key_con = 1;
	}
}


void Key3_Exit(void)
{
    if (Key_yes == 1)
	{
        Key_state = Key_state + 1;
	}
}

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_LineKEY1) == SET)		//判断是否是外部中断线触发的中断
	{
        uint32_t i = 2000000;
        while (i)
            i -= 1;
        Key1_Exit();
		EXTI_ClearITPendingBit(EXTI_LineKEY1);		//清除外部中断线的中断标志位
	}
}


void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_LineKEY2) == SET)		//判断是否是外部中断线触发的中断
	{
        uint32_t i = 2000000;
        while (i)
            i -= 1;
        Key2_Exit();
		EXTI_ClearITPendingBit(EXTI_LineKEY2);		//清除外部中断线的中断标志位
	}
}

void EXTI4_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_LineKEY3) == SET)		//判断是否是外部中断线触发的中断
	{
        uint32_t i = 2000000;
        while (i)
            i -= 1;
        Key3_Exit();
		EXTI_ClearITPendingBit(EXTI_LineKEY3);		//清除外部中断线的中断标志位
	}
}





