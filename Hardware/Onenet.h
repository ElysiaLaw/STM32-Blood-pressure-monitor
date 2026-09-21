#ifndef __ONENET_H
#define __ONENET_H

/********************************************
    注意，由于本函数多次运用了阻塞等待，因此推荐发送消息时使用循环收发
		即在已运行的循环中设置循环检查，如果有标志位则发送
        ************************************/


#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/********************************************
    全局Onenet启用变量，存在该宏定义以后其他相关部分会执行
        ************************************/
#define ONENET_HAVE


/********************************************
    全局Onenet收发相关数据存储
        ************************************/
#define ONENET_Client_ID "Esp_Link_HomeFirst"
#define ONENET_username "U4WXxev098"
#define ONENET_password "version=2018-10-31&res=products%2FU4WXxev098%2Fdevices%2FEsp_Link_HomeFirst&et=2145888000&method=md5&sign=1SFwuzzqjnqhahzH2UaX8A%3D%3D"


#define ONENET_publish_topic "$sys/U4WXxev098/Esp_Link_HomeFirst/thing/property/post"
#define ONENET_Subscribe_topic "$sys/U4WXxev098/Esp_Link_HomeFirst/thing/property/set"
#define ONENET_Reply_topic "$sys/U4WXxev098/Esp_Link_HomeFirst/thing/property/set_reply"


extern char OneNet_Tx[150];
extern char Onenet_RxPacket1[150]; 
extern char Onenet_RawJson[150];   // 完整json（用于autoreply返回消息）
extern char Onenet_PreTx[150];

extern uint8_t MQTT_PacketID;
extern uint8_t MQTT_Connected;
extern uint8_t MQTT_OtherNocon;
extern uint8_t MQTT_PubilshFlag;
extern uint8_t MQTT_ReceiveJson;
extern uint8_t MQTT_ReceiveFull;



void MYMQTT_Init(void);

void MYMQTT_Getconnect(void);


//底层接口函数，用于与实际硬件连接
//函数要求：输入参数1是要发送的数据，输入参数二是数据的长度，把数据发送出去
//要求：注意要在发送中禁止心跳包的发送，同时避免多个数据同时发送导致的重复错误
void MQTT_Send(char* OneNet_Tx, uint16_t len);



/*****************************************************/

//制作连接包，并输入到Rx缓冲区中中发出
void MQTT_Connect(char *client_id,char *username,char *password, uint16_t keepalive);

//制作心跳包，并输入到Rx缓冲区中中发出
void MQTT_Ping(void);

//制作订阅包，并输入到Rx缓冲区中发出
void MQTT_Subscribe(char *topic);

//制作发布包，并输入到Rx缓冲区中发出
void MQTT_Publish(char *topic, char *json);

//流式数据解析，可以放在串口终端中截取流式数据
void MQTT_StreamParse(uint8_t c);

//自动回复函数（注意，该函数需要放在主循环内调用）
void MQTT_AutoReply(void);

//工具函数，用于计算包长度
uint8_t MQTT_EncodeLength(uint8_t *buf, uint32_t length);


#endif



/*****************************************************

	将以下命令放到主循环中循环可以进行调试

uint8_t temp0 = 1;
char temp[150];
MYMQTT_Getconnect();

		MQTT_AutoReply();
		if (Key_con == 1)
		{
			sprintf(temp,"{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"temp\":{\"value\":%d}}}",temp0);
			MQTT_Publish(ONENET_publish_topic,temp);
			Key_con = 0;
			temp0 += 1;
		}
		
		
		if (MQTT_Connected == 1)
		{
			OLED_Clear();
			OLED_ShowStringpl(0,0,Serial_RxPacket1,OLED_8X16);
			OLED_Update();
			
		}
		
		
		if (MQTT_ReceiveJson == 1)
		{
			OLED_ShowStringpl(0,32,Onenet_RxPacket1,OLED_8X16);
			OLED_Update();
			MQTT_ReceiveJson = 0;
		}
		
		
*****************************************************/

