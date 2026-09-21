#ifndef __AD_H
#define __AD_H

#include "main.h"

/********************************************
    此定义启用后，PA2,PA3接口将被用来作为模拟电压输入口，参与AD
    注释后可以根据需要将这两个接口变为USART串口
        ************************************/
//#define MYAD_NOUSART

/********************************************
    全局模拟电压数值变量
        引脚分别为PA0,PA1,PA2,PA3
        ************************************/
extern uint16_t AD_Value[4];//AD.c



void AD_Init(void);

void AD_Start(void);

void AD_End(void);

#endif
