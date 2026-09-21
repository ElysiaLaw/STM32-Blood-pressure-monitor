#include "WIFI.h"

uint32_t MYWIFI_WAITStarttime = 0;
uint32_t MYWIFI_WAITEndtime = 0;

WIFIConnetStateType WIFI_Connet_State = WIFI_NotKnow;



/********************************************
	网络名称和密码存储
        ************************************/
char* WIFI_NAME = "LAW";
char* WIFI_CODE = "1766280w";



uint8_t WIFI_sendAT(char* ATcommand,char* ATanswer)
{
    uint32_t start_time;
    char WifiBuffer[WIFI_RX_BUFFER_SIZE];
    uint16_t current_len = 0;

    /* 发送指令 */
    if(ATcommand != NULL)
    {
        Serial_SendString(USART1, ATcommand);
    }

    /* 如果不需要检测返回 */
    if(ATanswer == NULL)
    {
        return 1;
    }
	
    Serial_RxFlag1 = 0;
    memset(WifiBuffer, 0, WIFI_RX_BUFFER_SIZE);
    current_len = 0;

    start_time = RTC_GetCounter();

    while((RTC_GetCounter() - start_time) < MYWIFI_MAXDELAY)
    {
        if(Serial_RxFlag1 == 1)
        {
            uint16_t new_len = strlen(Serial_RxPacket1);

            if(current_len + new_len < WIFI_RX_BUFFER_SIZE)
            {
                memcpy(&WifiBuffer[current_len],Serial_RxPacket1,new_len);
                current_len += new_len;
            }

            if(strstr(WifiBuffer, ATanswer) != NULL)
            {
                Serial_RxFlag1 = 0;
                return 1;
            }

            Serial_RxFlag1 = 0;
        }
    }

    return 0;
}

void WIFIErr_In(void)
{
	OLED_Update();
	while(1)
	{
		
	}
	
}

WIFIConnetStateType WIFI_ConnetStateCheck(void)
{
    char WifiBuffer[WIFI_RX_BUFFER_SIZE] = {0};
    uint16_t current_len = 0;
    uint32_t start_time;

    Serial_RxFlag1 = 0;
    memset(Serial_RxPacket1,0,sizeof(Serial_RxPacket1));

    WIFI_sendAT("AT+CIPSTATUS\r\n",NULL);

    start_time = RTC_GetCounter();

    while((RTC_GetCounter() - start_time) < MYWIFI_MAXDELAY)
    {
        if(Serial_RxFlag1 == 1)
        {
            uint16_t new_len = strlen(Serial_RxPacket1);

            if(current_len + new_len < WIFI_RX_BUFFER_SIZE - 1)
            {
                memcpy(&WifiBuffer[current_len],Serial_RxPacket1,new_len);
                current_len += new_len;
                WifiBuffer[current_len] = '\0';
            }

            Serial_RxFlag1 = 0;

            // 等到 OK 再统一判断
            if(strstr(WifiBuffer,"OK"))
            {
                /* ---------- 1. 是否连上 WiFi ---------- */
                if(strstr(WifiBuffer,"STATUS:5"))
                    return WIFI_NoWIFI;

                /* ---------- 2. 是否存在 TCP ---------- */
                if(strstr(WifiBuffer,"+CIPSTATUS:0,\"TCP\""))
                {
                    /* ---------- 3. TCP 是否已建立 ---------- */
                    if(strstr(WifiBuffer,",1\r\n") || strstr(WifiBuffer,",1"))
                        return WIFI_TCPing;   // 已连接服务器
                    else
                        return WIFI_NoTCP;    // 有 TCP 但已断
                }

                /* ---------- 4. 仅 WiFi 已连接 ---------- */
                if(strstr(WifiBuffer,"STATUS:2") || strstr(WifiBuffer,"STATUS:3")|| strstr(WifiBuffer,"STATUS:4"))
                    return WIFI_NoTCP;

                return WIFI_NotKnow;
            }
        }
    }

    return WIFI_NoHAVE; // 超时无响应
}


void WIFI_ATStart(void)
{
	/*****************************以下为相关变量的设置*****************************/
	MQTT_Connected = 0;
	
	
	/************************以下为模块起始命令************************************/
	uint8_t USART_RxState = 1;
	WIFIConnetStateType WIFI_Temp;
	char temp[150];
	Serial_Start(USART1);
	
	WIFI_sendAT("+++",NULL);
	Delay_ms(200);
	WIFI_sendAT("+++",NULL);
	Delay_ms(200);
	
	WIFI_Temp = WIFI_ConnetStateCheck();
	
	if (WIFI_Temp == WIFI_NotKnow)
	{
		OLED_ClearArea(0,12,128,24);
		if (USART_RxState == 1){USART_RxState = WIFI_sendAT("AT\r\n","OK");OLED_ShowString(0,12,"wifi start",OLED_8X16);}
			else {OLED_ShowString(0,12,"Err Reset 0",OLED_8X16);WIFIErr_In();}
		{OLED_Update();OLED_ClearArea(0,12,128,24);}
		//发送重置指令（注意，该指令会返回一大段无意义的乱码，而且不一定会有成功返回；因此手动置零标志位并延迟）
		if (USART_RxState == 1){USART_RxState = WIFI_sendAT("AT+RST\r\n",NULL);Delay_ms(2000);Serial_RxFlag1 = 0;}
			else {OLED_ShowString(0,12,"Err Reset 1",OLED_8X16);WIFIErr_In();}
		{OLED_Update();OLED_ClearArea(0,12,128,24);}
		
		//设置模式为对外链接wifi
		if (USART_RxState == 1){USART_RxState = WIFI_sendAT("AT+CWMODE=1\r\n","OK");OLED_ShowString(0,12,"Reset OK",OLED_8X16);}
			else {OLED_ShowString(0,12,"Err Reset 2",OLED_8X16);WIFIErr_In();}
		{OLED_Update();OLED_ClearArea(0,12,128,24);}
		
	}
	
	if ( WIFI_Temp == WIFI_NoWIFI || WIFI_Temp == WIFI_NotKnow)
	{
				//命令拼接
				sprintf(temp,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI_NAME,WIFI_CODE);
		
		if (USART_RxState == 1){USART_RxState = WIFI_sendAT(temp,"WIFI CONNECTED");OLED_ShowString(0,12,"connecting wifi",OLED_8X16);}
			else {OLED_ShowString(0,12,"Err Reset 3",OLED_8X16);WIFIErr_In();}
		{OLED_Update();OLED_ClearArea(0,12,128,24);}
		
		if (USART_RxState == 1){USART_RxState = WIFI_sendAT(NULL,"WIFI GOT IP");OLED_ShowString(0,12,"waiting..",OLED_8X16);}
			else {OLED_ShowString(0,12,"Err Connect 0",OLED_8X16);WIFIErr_In();}
		{OLED_Update();OLED_ClearArea(0,12,128,24);}
		

		if (USART_RxState == 1){USART_RxState = WIFI_sendAT("AT+CIPMUX=0\r\n","OK");OLED_ShowString(0,12,"wifi connected",OLED_8X16);}
			else {OLED_ShowString(0,12,"Err Connect 1",OLED_8X16);WIFIErr_In();}
		{OLED_Update();OLED_ClearArea(0,12,128,24);}
	}
	if (WIFI_Temp == WIFI_NoHAVE)
	{
		OLED_ShowString(0,12,"Maybe no CONNECT",OLED_8X16);
		WIFIErr_In();
	}
}


void WIFI_ATEnd(void)
{
	/************************以下为模块终止命令************************************/
	WIFI_sendAT("+++",NULL);
	Delay_ms(200);
	WIFI_sendAT("+++",NULL);
	{WIFI_sendAT("AT+CIPMODE=0\r\n","OK");}
	{WIFI_sendAT("AT+CIPCLOSE\r\n",NULL);
	
	//下面是终止提示（对部分命令有冲突）
	/*
	OLED_ShowString(0,12,"End",OLED_8X16);}
	{OLED_Update();OLED_ClearArea(0,12,128,24);
	*/
	}
	
	
	
}

void WIFI_Init(void)
{
    Serial_Init(USART1,115200);
}
