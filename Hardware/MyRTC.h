#ifndef __MYRTC_H
#define __MYRTC_H

#include "stm32f10x.h"                  // Device header
#include "main.h"

/******************************************
    时钟全局变量
        数组内容分别为年、月、日、时、分、秒
        ************************************/
extern uint16_t MyRTC_Time[];//MyRTC.c

void MyRTC_Init(void);
void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif
