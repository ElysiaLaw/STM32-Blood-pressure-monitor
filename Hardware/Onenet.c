#include "Onenet.h"


char OneNet_Tx[150];   // 发送缓存
char Onenet_RxPacket1[150]; //json提取文本的缓存
char Onenet_RawJson[150];   // 完整json（用于autoreply返回消息）
char Onenet_PreTx[150];		//预发送消息

uint8_t MQTT_PacketID = 1;   	// 报文ID
uint8_t MQTT_Connected = 0;   	// 是否已连接
uint8_t MQTT_OtherNocon = 0;	//禁止心跳包占用
uint8_t MQTT_PubilshFlag = 0;	//消息发送标志位
uint8_t MQTT_ReceiveJson = 0;	//接收到数据
uint8_t MQTT_ReceiveFull = 0;	//autoreply标志位（函数内自动判断，无需重复if判断）

void MQTT_Send(char* OneNet_Tx, uint16_t len)
{
	uint16_t i = 0;
	char temp[50];
	MQTT_OtherNocon = 1;
	//命令拼接
	sprintf(temp,"AT+CIPSEND=%d\r\n",len);
	
	WIFI_sendAT(temp,">");
	
		for (i = 0; i < len; i ++)		//遍历数组
			Serial_SendByte(USART1,OneNet_Tx[i]);		//依次调用Serial_SendByte发送每个字节数据
	
	WIFI_sendAT(temp,"SEND OK");
	MQTT_OtherNocon = 0;
}

void MYMQTT_Getconnect(void)
{
	uint8_t USART_RxState = 1;
	WIFIConnetStateType WIFI_Temp;
	uint8_t  type;
	
	WIFI_ATStart();
	Delay_s(2);
	
	WIFI_Temp = WIFI_ConnetStateCheck();
	if (WIFI_Temp == WIFI_TCPing)
		WIFI_ATEnd();
	
	WIFI_ATStart();
	
	if (USART_RxState == 1){USART_RxState = WIFI_sendAT("AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n","CONNECT");}
		else {OLED_ShowString(0,12,"Err Connect 2",OLED_8X16);WIFIErr_In();}
	{OLED_Update();OLED_ClearArea(0,12,128,24);}

	MQTT_Connect(ONENET_Client_ID,ONENET_username,ONENET_password,600);
	
	
	
	//处理返回信息，检查是否已连接
	char* temp;
	char* temp2;
	uint16_t len;
	uint32_t start_time;
	uint8_t check_return = 0;
	char OneNet_Rx[150];
	
	start_time =RTC_GetCounter();
	while((RTC_GetCounter() - start_time) < MYWIFI_MAXDELAY)
    {
        if(Serial_RxFlag1 == 1)
        {
			temp = strstr(Serial_RxPacket1, "+IPD,");
			if (temp)
			{
				//在这里进行收到数据以后的隔离处理
				len = atoi(temp + 5);
				temp2 = strchr(temp, ':');
				temp2++;
				memcpy(OneNet_Rx, temp2, len);
				
				//在这里检查是否连接成功
				type = OneNet_Rx[0] >> 4;
				if(OneNet_Rx[3] == 0x00 && type == 2)
				{
					MQTT_Subscribe(ONENET_Subscribe_topic);
					MQTT_Connected = 1;
					TIM_Start(TIM2);
				}
				
				//如果成功，跳出
				if (MQTT_Connected == 1)
					check_return = 1;
			}
            Serial_RxFlag1 = 0;
        }
		if (check_return)
			break;
    }
}

void MYMQTT_Init(void)
{
	Timer_Init();
}


uint8_t MQTT_EncodeLength(uint8_t *buf, uint32_t length)
{
    uint8_t i = 0;

    do
    {
        uint8_t byte = length % 128;
        length /= 128;

        if(length > 0)
            byte |= 0x80;

        buf[i++] = byte;

    } while(length > 0);

    return i;   // 返回用了几个字节
}

void MQTT_Connect(char *client_id,char *username,char *password, uint16_t keepalive)
{
    uint16_t i = 0;
    uint16_t len;
    uint16_t remain_len_index;
    uint16_t variable_start;

    OneNet_Tx[i++] = 0x10;    // CONNECT

    remain_len_index = i;     // 记录剩余长度位置
    i += 4;                   // 预留最大4字节空间

    variable_start = i;

    /* ---------- 协议头 ---------- */
    OneNet_Tx[i++] = 0x00;
    OneNet_Tx[i++] = 0x04;
    OneNet_Tx[i++] = 'M';
    OneNet_Tx[i++] = 'Q';
    OneNet_Tx[i++] = 'T';
    OneNet_Tx[i++] = 'T';

    OneNet_Tx[i++] = 0x04;    // MQTT 3.1.1

    OneNet_Tx[i++] = 0xC2;    // CleanSession + Username + Password

    OneNet_Tx[i++] = keepalive >> 8;
    OneNet_Tx[i++] = keepalive & 0xFF;

    /* ---------- Client ID ---------- */
    len = strlen(client_id);
    OneNet_Tx[i++] = len >> 8;
    OneNet_Tx[i++] = len & 0xFF;
    memcpy(&OneNet_Tx[i], client_id, len);
    i += len;

    /* ---------- Username ---------- */
    len = strlen(username);
    OneNet_Tx[i++] = len >> 8;
    OneNet_Tx[i++] = len & 0xFF;
    memcpy(&OneNet_Tx[i], username, len);
    i += len;

    /* ---------- Password ---------- */
    len = strlen(password);
    OneNet_Tx[i++] = len >> 8;
    OneNet_Tx[i++] = len & 0xFF;
    memcpy(&OneNet_Tx[i], password, len);
    i += len;

    /* ---------- 编码 Remaining Length ---------- */
    uint32_t remain_length = i - variable_start;

    uint8_t len_bytes =MQTT_EncodeLength(&OneNet_Tx[remain_len_index], remain_length);

    /* ---------- 数据前移 ---------- */
    if(len_bytes < 4)
    {
        memmove(&OneNet_Tx[remain_len_index + len_bytes],
                &OneNet_Tx[remain_len_index + 4],
                remain_length);
        i -= (4 - len_bytes);
    }
    MQTT_Send(OneNet_Tx, i);
}




void MQTT_Ping(void)
{
    OneNet_Tx[0] = 0xC0;
    OneNet_Tx[1] = 0x00;

    MQTT_Send(OneNet_Tx, 2);
}

void MQTT_Subscribe(char *topic)
{
    uint16_t i = 0;
    uint16_t remain_index;
    uint16_t variable_start;
    uint16_t topic_len = strlen(topic);

    OneNet_Tx[i++] = 0x82;  // SUBSCRIBE QoS1

    remain_index = i;
    i += 4;

    variable_start = i;

    /* 报文ID */
    OneNet_Tx[i++] = MQTT_PacketID >> 8;
    OneNet_Tx[i++] = MQTT_PacketID & 0xFF;
    MQTT_PacketID++;

    /* Topic */
    OneNet_Tx[i++] = topic_len >> 8;
    OneNet_Tx[i++] = topic_len & 0xFF;
    memcpy(&OneNet_Tx[i], topic, topic_len);
    i += topic_len;

    OneNet_Tx[i++] = 0x01;  // QoS1

    uint32_t remain_length = i - variable_start;

    uint8_t len_bytes =
        MQTT_EncodeLength(&OneNet_Tx[remain_index], remain_length);

    if(len_bytes < 4)
    {
        memmove(&OneNet_Tx[remain_index + len_bytes],
                &OneNet_Tx[remain_index + 4],
                remain_length);
        i -= (4 - len_bytes);
    }
    MQTT_Send(OneNet_Tx, i);
}

void MQTT_Publish(char *topic, char *json)
{
	if(MQTT_Connected == 0)
	{
		
		
		return;
	}
	
    uint16_t i = 0;
    uint16_t remain_index;
    uint16_t variable_start;

    uint16_t topic_len = strlen(topic);
    uint16_t data_len  = strlen(json);

    OneNet_Tx[i++] = 0x32;   // PUBLISH QoS1

    remain_index = i;
    i += 4;

    variable_start = i;

    /* Topic */
    OneNet_Tx[i++] = topic_len >> 8;
    OneNet_Tx[i++] = topic_len & 0xFF;
    memcpy(&OneNet_Tx[i], topic, topic_len);
    i += topic_len;

    /* Packet ID */
    OneNet_Tx[i++] = MQTT_PacketID >> 8;
    OneNet_Tx[i++] = MQTT_PacketID & 0xFF;
    MQTT_PacketID++;

    /* Payload */
    memcpy(&OneNet_Tx[i], json, data_len);
    i += data_len;

    uint32_t remain_length = i - variable_start;

    uint8_t len_bytes =
        MQTT_EncodeLength(&OneNet_Tx[remain_index], remain_length);

    if(len_bytes < 4)
    {
        memmove(&OneNet_Tx[remain_index + len_bytes],
                &OneNet_Tx[remain_index + 4],
                remain_length);
        i -= (4 - len_bytes);
    }
    MQTT_Send(OneNet_Tx, i);
	MQTT_PubilshFlag = 0;
}



void MQTT_StreamParse(uint8_t c)
{
    /* ---------- 完整JSON捕获 ---------- */

    static uint8_t json_capture = 0;
    static uint16_t json_index = 0;
    static uint16_t brace = 0;

    static char buf[150];

    if(json_capture == 0)
    {
        if(c == '{')
        {
            json_capture = 1;
            brace = 1;
            json_index = 0;

            buf[json_index++] = c;
        }
    }
    else
    {
        if(json_index < sizeof(buf)-1)
            buf[json_index++] = c;

        if(c == '{')
            brace++;

        if(c == '}')
        {
            brace--;

            if(brace == 0)
            {
                buf[json_index] = '\0';

                strcpy(Onenet_RawJson, buf);
                MQTT_ReceiveFull = 1;

                json_capture = 0;
            }
        }
    }


    /* ---------- params提取（原有功能保持） ---------- */

    static uint8_t params_capture = 0;
    static uint16_t params_index = 0;
    static uint16_t params_brace = 0;

    static char params_match[] = "\"params\":";
    static uint8_t match_index = 0;

    if(params_capture == 0)
    {
        if(c == params_match[match_index])
        {
            match_index++;

            if(params_match[match_index] == '\0')
            {
                params_capture = 1;
                match_index = 0;
            }
        }
        else
        {
            if(c == params_match[0])
                match_index = 1;
            else
                match_index = 0;
        }

        return;
    }

    if(params_brace == 0)
    {
        if(c == '{')
        {
            params_brace = 1;
            params_index = 0;

            Onenet_RxPacket1[params_index++] = c;
        }

        return;
    }

    if(params_index < sizeof(Onenet_RxPacket1)-1)
        Onenet_RxPacket1[params_index++] = c;

    if(c == '{')
        params_brace++;

    if(c == '}')
    {
        params_brace--;

        if(params_brace == 0)
        {
            Onenet_RxPacket1[params_index] = '\0';

            MQTT_ReceiveJson = 1;

            params_capture = 0;
            params_index = 0;
        }
    }
}

void MQTT_AutoReply(void)
{
    if(MQTT_ReceiveFull == 0)
        return;

    MQTT_ReceiveFull = 0;

    char id_value[20] = {0};

    char *id_start = strstr(Onenet_RawJson,"\"id\":\"");

    if(id_start == NULL)
        return;

    id_start += 6;

    char *id_end = strchr(id_start,'"');

    if(id_end == NULL)
        return;

    uint16_t len = id_end - id_start;

    if(len >= sizeof(id_value))
        return;

    memcpy(id_value,id_start,len);
    id_value[len] = '\0';

    char reply[80];

    sprintf(reply,
    "{\"id\":\"%s\",\"code\":0,\"msg\":\"success\"}",
    id_value);

    MQTT_Publish(ONENET_Reply_topic,reply);
}
