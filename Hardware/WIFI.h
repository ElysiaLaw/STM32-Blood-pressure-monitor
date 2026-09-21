#ifndef __WIFI_H
#define __WIFI_H

#include "main.h"
#include <string.h>

/********************************************
    全局最大超时等待变量
        ************************************/
extern uint32_t MYWIFI_WAITStarttime;
extern uint32_t MYWIFI_WAITEndtime;

/********************************************
    全局最大超时时间定义(单位：秒)
        ************************************/
#define MYWIFI_MAXDELAY 15


/********************************************
    此定义启用后，其他菜单界面的联网函数会启用，否则失效
        ************************************/
#define HAVE_WIFI

/********************************************
    联网获取数据帧最大帧长度
        ************************************/
#define WIFI_RX_BUFFER_SIZE 256

/********************************************
	网络名称和密码存储（注意，相关数据修改在对应的c文件中）
        ************************************/
extern char* WIFI_NAME;
extern char* WIFI_CODE;


typedef enum
{ WIFI_NotKnow = 0,		//0，1
  WIFI_NoWIFI,			//5
  WIFI_NoTCP,			//2，4
  WIFI_NoInit,			//
  WIFI_TCPing,			//3
  WIFI_NoHAVE,
} WIFIConnetStateType;


/********************************************
	WIFI连接状态
        ************************************/
extern WIFIConnetStateType WIFI_Connet_State;

void WIFI_Init(void);
void WIFI_ATStart(void);
void WIFI_ATEnd(void);

//错误反馈调试指令
void WIFIErr_In(void);
//硬件接口指令
uint8_t WIFI_sendAT(char* ATcommand,char* ATanswer);
//状态查询指令
WIFIConnetStateType WIFI_ConnetStateCheck(void);

#endif
