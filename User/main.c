#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "main.h"

//全局按键变量
uint8_t Key_con = 0;
uint8_t Key_state = 1;

//全局菜单变量
uint8_t menu_state = 255;


float temp = 0;

int main(void)
{
	UI_Init();
	Key_state = 1;
	UI_now = 1 * 48;
	AD_Start();
	
	
	BloodPressure_DebugMeasure();
	
	OLED_Clear();
	OLED_Printfpl(0,12,OLED_8X16,"END",High_Pressure,Low_Pressure,Heartbeat);
	OLED_Printfpl(0,0,OLED_8X16,"SYS:%d DIA:%d HR:%d ||",High_Pressure,Low_Pressure,Heartbeat);
	OLED_Update();
	while (1)
	{
//		temp = Get_mmhg();
//		OLED_Clear();
//		OLED_Printfpl(0,12,OLED_8X16,"%.2f",temp);
//		
//		OLED_Update();
		
		
		
//		OLED_Clear();
//        Key_con = 0;
//		if (menu_state == ADSHOWUI_UInum)
//			ADSHOWUI_In();
//		else
//			ADUI_In();
//		
/**************************主循环开始************************/

//        OLED_Clear();
//        Key_con = 0;
//        switch (menu_state)
//        {
//			case menuUI_UInum:
//                menuUI_In();
//                break;
//            case ADUI_UInum:
//                ADUI_In();
//                break;
//			case ADSHOWUI_UInum:
//                ADSHOWUI_In();
//				break;
//			case CheckUI_UInum:
//                CheckUI_In();
//				break;
//			case SettingUI_UInum:
//                SettingUI_In();
//				break;
//			case ConfigUI_UInum:
//                ConfigUI_In();
//				break;
//			
//			
//            default:
//                menuUI_In();
//                break;
//        }

/**************************主循环结束************************/

    }
}



