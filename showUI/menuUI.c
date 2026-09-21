#include "menuUI.h"

uint16_t UI_now;
uint16_t UI_last;

/********************************************
    菜单界面数量
        ************************************/
uint8_t number = 4;

/********************************************
    ui动画移动速度
        匀速动画下该数值必须能够整除48且不得超过48才能有效,数值越大越快
        非匀速动画尽量选择能被整除的多的，数值越小越快
        ************************************/
uint8_t UI_V = 4;


/********************************************
    ui界面中文字幕
        ************************************/
		char* menu_chinese[] = {"血压测量","相关设置","进入调试","模拟显示"};

/********************************************
    ui动画是否是非匀速的
        ************************************/
#define menuUI_ACC//该函数注释掉就是匀速动画，不注释就是非匀速动画


void UI_Init(void)
{
    OLED_Init();
    Key_Init();
    ADUI_Init();
    WIFI_Init();
	
}

uint16_t reduct(uint16_t a,uint16_t b)
{
    uint16_t i;
    if (a >= b)
        i = a - b ;
    else if(a <= b)
        i = b - a ;
    return i;
}

void menuUI_In(void)
{
    //进入页面时的初始化（防止动画加载）
    UI_last = UI_now;
    //设置进入页面时的按键状态
    
    //设置循环，完成页面内交互
    while(Key_con == 0)
    {
        //限制按键状态，防止溢出
        Delay_ms(5);
        if (Key_state < 1 )
            Key_state = number;
        if (Key_state > number )
            Key_state = 1;
        //修改当前值，为下面刷新提供终点参数
        UI_now = 48 * Key_state;
        //页面内显示刷新
        menuUI_Update();
        
    }
    //页面交互完毕后的页面跳转
    switch (Key_state)
    {
        case CheckUI_UInum:
            menu_state = CheckUI_UInum;
            break;
        case SettingUI_UInum:
            menu_state = SettingUI_UInum;
            break;
		case ConfigUI_UInum:
            menu_state = ConfigUI_UInum;
            break;
		case ADUI_UInum:
            menu_state = ADUI_UInum;
            break;
		
        default:
           menu_state = menuUI_UInum;
    }
    Key_state = 1;
    OLED_Clear();
    OLED_Update();
}


void menuUI_Update(void)
{
#ifdef menuUI_ACC
    uint8_t i = 7;//非匀速动画下可行的最大速度
    uint8_t move ;
#endif
    
    OLED_ShowImage(39,8,48,40,UIout);
    OLED_ClearArea(40,52,48,12);
    
    if (UI_now != UI_last)
        Key_yes = 0;
    while(UI_now != UI_last)
    {
        menuUI_inUp();
#ifdef menuUI_ACC
        if (reduct(UI_now,UI_last) >= UI_V)
        {
            move = (uint8_t)(reduct(UI_now,UI_last) / UI_V);
            if(move > i)
                move = i;
            
            if (UI_now > UI_last)
                UI_last += move;
            if (UI_now < UI_last)
                UI_last -= move;
        }
#endif
#ifndef menuUI_ACC
        if ((reduct(UI_now,UI_last) % UI_V == 0) && (48 >= UI_V))
        {
            if (UI_now > UI_last)
                UI_last += UI_V;
            if (UI_now < UI_last)
                UI_last -= UI_V;
        
        }
#endif
        else
        {
            if (UI_now > UI_last)
                UI_last += 1 ;
            if (UI_now < UI_last)
                UI_last -= 1;
        }
    }
    OLED_Printf(40,52,OLED_6X8,"%s",menu_chinese[UI_now/48 - 1]);
    
    menuUI_inUp();
    Key_yes = 1;
}

void menuUI_inUp(void)
{
    uint8_t i;
    
    for(i = 2; i < number  + 2; i+= 1)
        if (((i * 48 - UI_last - 1) >= -32) &&((i * 48 - UI_last - 1) <= 160))
            OLED_ShowImage(i * 48 - UI_last - 1,11,32,32,MyRTC_UI[i - 2]);
    for(i = 2; i < number  + 3; i+= 1)
        if (((i * 48 - UI_last - 1) >= -32) &&((i * 48 - UI_last - 1) <= 160))
            OLED_ClearArea(i * 48 - UI_last - 16 - 1,11,16,32);
    
    OLED_Update();
}


