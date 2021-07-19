#include "STM32_head.h"
#include "STM32_usart.h"
/***********************************
file：STM32_usart.c
作者：廖凌峰
时间：2019/7/16
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
//			支持串口：
//      USART1_TX ------ PA9       //
//      USART1_RX ----- PA10       //
//      USART2_TX ----- PA2        //
//      USART2_RX ----- PA3        //
//      USART3_TX ----- PB10       //
//      USART3_RX ----- PB11       //
#define NULL 0
#if ENABLE
char BUFF[LENGTH]={0};
#endif
/************************************
函数名:USART_init
描述：串口初始化
输入：USARTx(串口),pick(时钟),bound(波特率),i(是否使能串口中断)
输出：无
样例：USART_init(USART1,72,9600,ENABLE);//USART1,72MHZ,波特率9600,使能串口中断
***********************************/
void USART_init(USART_TypeDef * USARTx,uint32_t pick,uint32_t bound,u16 i)
{
	float temp;
	u16 zhengshu;//整数部分
	u16 xiaoshu;//小数部分
	GPIO_USART_IOint(USARTx);
	//调制波特率
	temp=(float)(pick*1000000)/(16*bound);//USARTDIV=(外设时钟频率)/(16*波特率)   见STM32中文参考手册25.3.4
	zhengshu=temp;//整数部分
	xiaoshu=(temp-zhengshu)*16;//小数部分
	zhengshu<<=4;
	zhengshu+=xiaoshu;
	USARTx->BRR=zhengshu;//波特率设置
	USARTx->CR1|=(1<<13)|(1<<3)|(1<<2);//一个起始位，一个停止位，无奇偶校验
	if(i)
	{
		USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
		//MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级
	}
}
/************************************
函数名:USART_Get_state
描述：检测串口是否有数据传来
输入：USARTx(串口)
输出：uint8_t
样例：USART_Get_state(USART1);
***********************************/
uint8_t USART_Get_state(USART_TypeDef * USARTx)
{
	if(USARTx->SR&(1<<5))//数据寄存器有数据
		return 1;
	else
		return 0;
}
/************************************
函数名:USART_Sent_state
描述：检测串口是否将数据发出
输入：USARTx(串口)
输出：uint8_t
样例：USART_Sent_state(USART1);
***********************************/
uint8_t USART_Sent_state(USART_TypeDef * USARTx)
{
	if(USARTx->SR&(1<<6))//数据寄存器没有数据，已发出
		return 1;
	else
		return 0;
}
/************************************
函数名:USART_Get_char
描述：串口数据获取
输入：USARTx(串口)
输出：uint8_t
样例：USART_Get_char(USART1);//USART1数据获取
***********************************/
uint8_t USART_Get_char(USART_TypeDef * USARTx)
{
	while(!(USARTx->SR&(1<<5)));//等待数据寄存器不为空（即有数据）
	return USARTx->DR;//接收数据
}
/************************************
函数名:USART_Get_String
描述：串口数据获取
输入：USARTx(串口)
输出：uint8_t
USART_Get_String(USART1,BUFF[]);//USART1数据获取
***********************************/
void USART_Get_String(USART_TypeDef * USARTx,char *p)
{
	int i;
	char *q = NULL;
	char *l = p;
	for(i=0;i<LENGTH;i++)//清空BUFF区
	{
		*l = 0;
		l++;
	}
	while(1)
	{
		*p = USART_Get_char(USARTx);
		if(*p == 0x0a && *q == 0x0d)
		{
			break;
		}
		q = p;//前驱
		p++;
	}
}
/************************************
函数名:USART_Sent_char
描述：串口数据发送
输入：USARTx(串口),RT(数据)
输出：无
样例：USART_Sent_char(USART1,0x30);//USART1数据发送
***********************************/
void USART_Sent_char(USART_TypeDef * USARTx,uint8_t RT)//发送字符
{
	while(!(USARTx->SR&(1<<6)));//等待数据发送完成（即没有数据）
	USARTx->DR=RT;//发送数据
}
/************************************
函数名:USART_Sent_String
描述：串口数据发送
输入：USARTx(串口),*p(字符串地址),q(是否换行1:换0:不换)
输出：无
样例：USART_Sent_String(USART1,"sda",1);//USART1数据发送换新行
***********************************/
void USART_Sent_String(USART_TypeDef * USARTx,char *p,char q)//发送字符串
{
	while(*p)
	{
		USART_Sent_char(USARTx,*p);
		p++;
	}
	if(q)
	{
		USART_Sent_char(USARTx,0x0d);
		USART_Sent_char(USARTx,0x0a);
	}
}
/************************************
函数名:USART_Sent_nbit
描述：串口数据发送
输入：USARTx(串口),*p(字符串地址),n(发送字节数)
输出：无
样例：USART_Sent_nbit(USART1,"sda",3);//USART1数据发送换n个字节
***********************************/
void USART_Sent_nbit(USART_TypeDef * USARTx,char *p,int n)
{
	while(n--)
	{
		USART_Sent_char(USARTx,*p);
		p++;
	}
}
/************************************
函数名:USART_show
描述：串口发送
输入：i(串口几),
输出：无
样例：USART_show(1);
***********************************/
USART_TypeDef * USART_show(char i)
{
	switch (i)
	{
		case '1':return USART1;break;
		case '2':return USART2;break;
		case '3':return USART3;break;
	}
	return 0;
}

