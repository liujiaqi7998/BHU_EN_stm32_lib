#ifndef _HIM_H
#define _HIM_H
#include "sys.h"

void init_HMI(USART_TypeDef * USARTX);
void clear(USART_TypeDef * USARTX);
void zhilin(USART_TypeDef * USARTX,char *p);
void display(int chose,USART_TypeDef * USARTX,char *p,char *l,int h);
void numchuan(USART_TypeDef * USARTX,int p);
void get(USART_TypeDef * USARTXx1,USART_TypeDef * USARTXx2,char *p);
void recivehim(USART_TypeDef * USARTx1,USART_TypeDef * USARTx2);
#endif

























