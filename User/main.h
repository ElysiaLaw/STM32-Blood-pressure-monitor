#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "MyRTC.h"
#include "AD.h"
#include "menuUI.h"
#include "ADUI.h"
#include "Serial.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "WIFI.h"
#include "Onenet.h"
#include "Timer.h"
#include "OTHERUI.h"
#include "Xueya.h"
#include <stdint.h>

/********************************************
    全局菜单变量,从1开始计数，
            如需修改，直接改数就行了，然后把menu改一下就行
                注意：为防止出现bug，请将数值按照菜单顺序进行修改
                未出现在菜单上的页面可以随便填
        1、时间界面
        2、ADC转换
        3、联网查询
        4、实时监测
		5、云平台控制
        ************************************/
extern uint8_t menu_state;//main.c

#define ADUI_UInum 4               //实时时钟界面
#define menuUI_UInum 255            //菜单界面
#define CheckUI_UInum 1			//ADC波形显示
#define SettingUI_UInum 2
#define ConfigUI_UInum 3
#define ADSHOWUI_UInum 254




#endif
