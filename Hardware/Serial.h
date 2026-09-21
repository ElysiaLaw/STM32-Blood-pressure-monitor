#ifndef __SERIAL_H
#define __SERIAL_H

#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/********************************************
    该定义为接收数据的数组的长度
        ************************************/
#define MYSerial_RxMAX 150



//自定义量，为初始化函数提供usart的串口选择
typedef enum
{ MYUSART1,
  MYUSART2,
  MYUSART3
} MYUsart_Typedef;


/********************************************
    全局串口接收变量函数
        ************************************/
extern char Serial_RxPacket1[MYSerial_RxMAX];				//定义接收数据包数组，数据包无格式
extern char Serial_RxPacket2[MYSerial_RxMAX];				//定义接收数据包数组，数据包无格式
extern char Serial_RxPacket3[MYSerial_RxMAX];				//定义接收数据包数组，数据包无格式
extern uint8_t Serial_RxFlag1;					//定义接收数据包标志位
extern uint8_t Serial_RxFlag2;					//定义接收数据包标志位
extern uint8_t Serial_RxFlag3;					//定义接收数据包标志位




void Serial_ALLInit(uint32_t MYUSART_BaudRate);
void Serial_Init(USART_TypeDef* USARTx,uint32_t MYUSART_BaudRate);
void Serial_Start(USART_TypeDef* USARTx);
void Serial_End(USART_TypeDef* USARTx);
void Serial_SendByte(USART_TypeDef* USARTx,uint8_t Byte);
void Serial_SendArray(USART_TypeDef* USARTx,uint8_t *Array, uint16_t Length);
void Serial_SendString(USART_TypeDef* USARTx,char *String);
void Serial_SendNumber(USART_TypeDef* USARTx,uint32_t Number, uint8_t Length);
void Serial_Printf(USART_TypeDef* USARTx,char *format, ...);


/**********************************************/
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
int fputc(int ch, FILE *f);

#endif
