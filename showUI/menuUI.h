#ifndef __MENUUI_H
#define __MENUUI_H

#include "main.h"

/********************************************
    菜单界面位置全局存储
        ************************************/
extern uint16_t UI_now;
extern uint16_t UI_last;


void UI_Init(void);
void menuUI_In(void);
void menuUI_Update(void);
void menuUI_inUp(void);


uint16_t reduct(uint16_t a,uint16_t b);//求二者的差值

#endif
