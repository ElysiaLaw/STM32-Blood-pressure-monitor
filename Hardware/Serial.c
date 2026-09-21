#include "Serial.h"


char Serial_RxPacket1[MYSerial_RxMAX];				//定义接收数据包数组，数据包格式"MSG\r\n"
char Serial_RxPacket2[MYSerial_RxMAX];				//定义接收数据包数组，数据包格式"@MSG\r\n"
char Serial_RxPacket3[MYSerial_RxMAX];				//定义接收数据包数组，数据包格式"@MSG\r\n"
uint8_t Serial_RxFlag1;					//定义接收数据包标志位
uint8_t Serial_RxFlag2;					//定义接收数据包标志位
uint8_t Serial_RxFlag3;					//定义接收数据包标志位


volatile uint8_t RxState1 = 0;		//定义表示当前状态机状态的静态变量
volatile uint8_t pRxPacket1 = 0;	//定义表示当前接收数据位置的静态变量

volatile uint8_t RxState2 = 0;		//定义表示当前状态机状态的静态变量
volatile uint8_t pRxPacket2 = 0;	//定义表示当前接收数据位置的静态变量

volatile uint8_t RxState3 = 0;		//定义表示当前状态机状态的静态变量
volatile uint8_t pRxPacket3 = 0;	//定义表示当前接收数据位置的静态变量


void Serial_ALLInit(uint32_t MYUSART_BaudRate)
{
    Serial_Init(USART1,MYUSART_BaudRate);
    Serial_Init(USART2,MYUSART_BaudRate);
    Serial_Init(USART3,MYUSART_BaudRate);
}

/**
  * 函    数：串口初始化
  * 参    数：无
  * 返 回 值：无
  */
void Serial_Init(USART_TypeDef* USARTx ,uint32_t MYUSART_BaudRate)
{
    if (USARTx == USART1)
    {
        /*开启时钟*/
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//开启USART1的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
        
        /*GPIO初始化*/
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA9引脚初始化为复用推挽输出
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA10引脚初始化为上拉输入
        
        /*USART初始化*/
        USART_InitTypeDef USART_InitStructure;					//定义结构体变量
        USART_InitStructure.USART_BaudRate = MYUSART_BaudRate;				//波特率
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
        USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
        USART_Init(USART1, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
        
        /*中断输出配置*/
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
        USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

		
        /*NVIC中断分组*/
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
        
        /*NVIC配置*/
        NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//选择配置NVIC的USART1线
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
        NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
    }

    if (USARTx == USART2)
    {
        /*开启时钟*/
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//开启USART1的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIOA的时钟
        
        /*GPIO初始化*/
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA2引脚初始化为复用推挽输出
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA3引脚初始化为上拉输入
        
        /*USART初始化*/
        USART_InitTypeDef USART_InitStructure;					//定义结构体变量
        USART_InitStructure.USART_BaudRate = MYUSART_BaudRate;				//波特率
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
        USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
        USART_Init(USART2, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART2
        
        /*中断输出配置*/
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
        USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
		
        /*NVIC中断分组*/
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
        
        /*NVIC配置*/
        NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//选择配置NVIC的USART1线
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
        NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
    }
    
    if (USARTx == USART3)
    {
        /*开启时钟*/
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//开启USART1的时钟
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOA的时钟
        
        /*GPIO初始化*/
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);					//将PA9引脚初始化为复用推挽输出
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);					//将PA10引脚初始化为上拉输入
        
        /*USART初始化*/
        USART_InitTypeDef USART_InitStructure;					//定义结构体变量
        USART_InitStructure.USART_BaudRate = MYUSART_BaudRate;				//波特率
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制，不需要
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//模式，发送模式和接收模式均选择
        USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验，不需要
        USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位，选择1位
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长，选择8位
        USART_Init(USART3, &USART_InitStructure);				//将结构体变量交给USART_Init，配置USART1
        
        /*中断输出配置*/
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//开启串口接收数据的中断
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
        
//        /*NVIC中断分组*/
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//配置NVIC为分组2
        
        /*NVIC配置*/
        NVIC_InitTypeDef NVIC_InitStructure;					//定义结构体变量
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		//选择配置NVIC的USART1线
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//指定NVIC线路使能
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//指定NVIC线路的抢占优先级为1
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//指定NVIC线路的响应优先级为1
        NVIC_Init(&NVIC_InitStructure);							//将结构体变量交给NVIC_Init，配置NVIC外设
    }
}


void Serial_Start(USART_TypeDef* USARTx)
{
    USART_Cmd(USARTx, ENABLE);
}

void Serial_End(USART_TypeDef* USARTx)
{
    USART_Cmd(USARTx, DISABLE);
}
/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte(USART_TypeDef* USARTx,uint8_t Byte)
{
	USART_SendData(USARTx, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	//等待发送完成
	/*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Serial_SendByte(USARTx,Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(USART_TypeDef* USARTx,char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(USARTx,String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}



/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)		//根据数字长度遍历数字的每一位
	{
		Serial_SendByte(USARTx,Number / Serial_Pow(10, Length - i - 1) % 10 + '0');	//依次调用Serial_SendByte发送每位数字
	}
}



/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(USART_TypeDef* USARTx,char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(USARTx,String);		//串口发送字符数组（字符串）
}



/**
  * 函    数：USART1中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void USART1_IRQHandler(void)
{
	uint32_t temp;
	
	//判断是否是USART1的接收事件触发的中断，执行数据接收
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);			//读取数据寄存器，存放在接收的数据变量

#ifdef ONENET_HAVE
		MQTT_StreamParse(RxData);
#endif
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据*/
		if (RxState1 == 0)
		{
			if (Serial_RxFlag1 == 0)		//如果上一个数据包已处理完毕
			{
				pRxPacket1 = 0;			//数据包的位置归零
                Serial_RxPacket1[pRxPacket1] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket1 ++;			//数据包的位置自增
                RxState1 = 1;			//置下一个状态
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState1 == 1)
		{
			if (pRxPacket1 < MYSerial_RxMAX)		//接收到了正常的数据且没有溢出
			{
				Serial_RxPacket1[pRxPacket1] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket1 ++;			//数据包的位置自增
			}
            else                        //数据溢出，直接删除
            {
                pRxPacket1 = 0;
                Serial_RxPacket1[pRxPacket1] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket1 ++;			//数据包的位置自增
            }
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);		//清除标志位
	}
	
	//判断是否是USART1的接收空闲中断，执行接收终止
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		RxState1 = 0;			//状态归0
		Serial_RxPacket1[pRxPacket1] = '\0';			//将收到的字符数据包添加一个字符串结束标志
		Serial_RxFlag1 = 1;		//接收数据包标志位置1，成功接收一个数据包
		
		temp = USART1->SR;
		temp = USART1->DR;		//清除标志位
	}
}


void USART2_IRQHandler(void)
{
	uint32_t temp;
	
	//判断是否是USART1的接收事件触发的中断，执行数据接收
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART2);			//读取数据寄存器，存放在接收的数据变量
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据*/
		if (RxState2 == 0)
		{
			if (Serial_RxFlag2 == 0)		//如果上一个数据包已处理完毕
			{
				pRxPacket2 = 0;			//数据包的位置归零
                Serial_RxPacket2[pRxPacket2] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket2 ++;			//数据包的位置自增
                RxState2 = 1;			//置下一个状态
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState2 == 1)
		{
			if (pRxPacket2 < MYSerial_RxMAX)		//接收到了正常的数据且没有溢出
			{
				Serial_RxPacket2[pRxPacket2] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket2 ++;			//数据包的位置自增
			}
            else                        //数据溢出，直接删除
            {
                pRxPacket2 = 0;
                Serial_RxPacket2[pRxPacket2] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket2 ++;			//数据包的位置自增
            }
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		//清除标志位
	}
	
	//判断是否是USART1的接收空闲中断，执行接收终止
	if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{
		RxState2 = 0;			//状态归0
		Serial_RxPacket2[pRxPacket2] = '\0';			//将收到的字符数据包添加一个字符串结束标志
		Serial_RxFlag2 = 1;		//接收数据包标志位置1，成功接收一个数据包
		
		temp = USART2->SR;
		temp = USART2->DR;		//清除标志位
	}
}




void USART3_IRQHandler(void)
{
	uint32_t temp = 0;
	
	//判断是否是USART1的接收事件触发的中断，执行数据接收
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART3);			//读取数据寄存器，存放在接收的数据变量
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据*/
		if (RxState3 == 0)
		{
			if (Serial_RxFlag3 == 0)		//如果上一个数据包已处理完毕
			{
				pRxPacket3 = 0;			//数据包的位置归零
                Serial_RxPacket3[pRxPacket3] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket3 ++;			//数据包的位置自增
                RxState3 = 1;			//置下一个状态
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState3 == 1)
		{
			if (pRxPacket3 < MYSerial_RxMAX)		//接收到了正常的数据且没有溢出
			{
				Serial_RxPacket3[pRxPacket3] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket3 ++;			//数据包的位置自增
			}
            else                        //数据溢出，直接删除
            {
                pRxPacket3 = 0;
                Serial_RxPacket3[pRxPacket3] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket3 ++;			//数据包的位置自增
            }
		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);		//清除标志位
	}
	
	//判断是否是USART1的接收空闲中断，执行接收终止
	if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
	{
		RxState3 = 0;			//状态归0
		Serial_RxPacket3[pRxPacket3] = '\0';			//将收到的字符数据包添加一个字符串结束标志
		Serial_RxFlag3 = 1;		//接收数据包标志位置1，成功接收一个数据包
		
		temp = USART3->SR;
		temp = USART3->DR;		//清除标志位
	}
}



/**************************************************************/

/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;	//设置结果初值为1
	while (Y --)			//执行Y次
	{
		Result *= X;		//将X累乘到结果
	}
	return Result;
}


/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
	Serial_SendByte(USART1,ch);			//将printf的底层重定向到自己的发送字节函数
	return ch;
}
