#ifndef __STM32_usart_H
#define __STM32_usart_H
#include <stm32f10x.h>
#define ENABLE 1
#define DISENABLE 0

#define LENGTH 200   //�����������ֽ��� 200
extern char BUFF[LENGTH];
void USART_init(USART_TypeDef * USARTx,uint32_t pick,uint32_t bound,u16 i);
uint8_t USART_Get_state(USART_TypeDef * USARTx);
uint8_t USART_Sent_state(USART_TypeDef * USARTx);
uint8_t USART_Get_char(USART_TypeDef * USARTx);
void USART_Get_String(USART_TypeDef * USARTx,char *p);
void USART_Sent_char(USART_TypeDef * USARTx,uint8_t RT);//�����ַ�
void USART_Sent_String(USART_TypeDef * USARTx,char *p,char q);//�����ַ���
USART_TypeDef * USART_show(char i);
void USART_Sent_nbit(USART_TypeDef * USARTx,char *p,int n);//USARTx���ݷ��ͻ�n���ֽ�



#endif


