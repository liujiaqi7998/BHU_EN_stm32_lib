#ifndef __STM32_usart_H
#define __STM32_usart_H
#include <stm32f10x.h>
#define ENABLE 1
#define DISENABLE 0

#define LENGTH 200   //定义最大接收字节数 200
extern char BUFF[LENGTH];
void USART_init(USART_TypeDef * USARTx,uint32_t pick,uint32_t bound,u16 i);
uint8_t USART_Get_state(USART_TypeDef * USARTx);
uint8_t USART_Sent_state(USART_TypeDef * USARTx);
uint8_t USART_Get_char(USART_TypeDef * USARTx);
void USART_Get_String(USART_TypeDef * USARTx,char *p);
void USART_Sent_char(USART_TypeDef * USARTx,uint8_t RT);//发送字符
void USART_Sent_String(USART_TypeDef * USARTx,char *p,char q);//发送字符串
USART_TypeDef * USART_show(char i);
void USART_Sent_nbit(USART_TypeDef * USARTx,char *p,int n);//USARTx数据发送换n个字节



#endif


