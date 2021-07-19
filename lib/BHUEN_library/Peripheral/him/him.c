#include "him.h"
#include "STM32_head.h"
/***********************************
file：him.c
作者：张柏(skr)
时间：2019/7/18
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
//			支持串口：
//      USART1_TX ------ PA9       //
//      USART1_RX ----- PA10       //
//      USART2_TX ----- PA2        //
//      USART2_RX ----- PA3        //
//      USART3_TX ----- PB10       //
//      USART3_RX ----- PB11       //
/************************************
函数名:init_HMI
描述：串口屏初始化
输入：USARTx(串口)
输出：无
样例：init_HMI(USART1);//USART1串口上的串口屏初始化
***********************************/
void init_HMI(USART_TypeDef * USARTX)
{
	delay_ms(200);
	USART_Sent_char(USARTX,0xff);
	delay_ms(200);	delay_ms(200);
	delay_ms(200);

}
/************************************
函数名:clear
描述：串口屏清屏
输入：USARTx(串口)
输出：无
样例：init_HMI(USART1);//USART1串口上的串口屏清屏
***********************************/
void clear(USART_TypeDef * USARTX)
{   
    char p[10]="page 0";
    zhilin(USARTX,p);
}
/************************************
函数名:zhilin
描述：给串口屏指令
输入：USARTx(串口)
输出：无
样例：zhilin(USART1,"t1.txt"))//获取串口一上的串口屏t1.txt
***********************************/
void zhilin(USART_TypeDef * USARTX,char *p)//给串口屏写指令
{
	        USART_Sent_String(USARTX,p,0);
	        USART_Sent_char(USARTX,0xff);
		    USART_Sent_char(USARTX,0xff);
	        USART_Sent_char(USARTX,0xff);
}
/************************************
函数名:display
描述：把一些数据在串口屏上面显示
输入：chose(选择要传的数据类型0为数字1为字符2为数字加字符)，USARTx(串口，要执行的指令，字符串，数字)；
输出：无
样例：display(1，USART1,"t1.txt"，“2”，p))//传字符串2到t1.txt
***********************************/
void display(int chose,USART_TypeDef * USARTX,char *p,char *l,int h)
{ 
	  if(chose==0)
		 {
		    USART_Sent_String(USARTX,p,0);
	        USART_Sent_String(USARTX,"\"",0);  
	        numchuan(USARTX,h);
		    USART_Sent_String(USARTX,"\"",0); 
	        USART_Sent_char(USARTX,0xff);
		    USART_Sent_char(USARTX,0xff);
	        USART_Sent_char(USARTX,0xff);
		 } 
 	   if(chose==1)
		 {
		    USART_Sent_String(USARTX,p,0);
	        USART_Sent_String(USARTX,"\"",0);  
	        USART_Sent_String(USARTX,l,0);
		    USART_Sent_String(USARTX,"\"",0); 
	        USART_Sent_char(USARTX,0xff);
		    USART_Sent_char(USARTX,0xff);
	        USART_Sent_char(USARTX,0xff);
		 } 	
		 if(chose==2)	
		 {
				USART_Sent_String(USARTX,p,0);
				USART_Sent_String(USARTX,"\"",0); 
					numchuan(USARTX,h);		  
				USART_Sent_String(USARTX,l,0);
			USART_Sent_String(USARTX,"\"",0);
				USART_Sent_char(USARTX,0xff);
			USART_Sent_char(USARTX,0xff);
				USART_Sent_char(USARTX,0xff);  
			}		  
}
/************************************
函数名:numchuan
描述：对要传输的数字数组进行一些处理
输入：numchuan((串口，数字数组)；
输出：无
样例：display(USART1,p))//对p数组中的每个元素进行拆分处理
***********************************/
void numchuan(USART_TypeDef * USARTX,int p)
{   
	int i;//循环处理用的数
	int k;//记录数字是几位的
	int num[100];//用一个数组存储拆解完的数
	int flag;//用这个数做拆解
	int i1;//for循环使用
			i=0;
		    flag=p;
		    k=0;
		while(flag)
		{
		 num[i]=(flag%10);
		flag=flag/10;
			i++;
			k++;
		}
		i=i-1;
		for(i1=0;i1<k;i1++)
		{
		    USART_Sent_char(USARTX,num[i]+0x30);
			num[i]=0x00;
			i--;
		}		
}
/************************************
函数名:get
描述：获取屏幕数据 想要什么数据就get 啥 例如 get t1.txt
输入：get((串口屏所在的串口，给电脑发数据的串口，相应的指令)；
输出：无
样例：get(USART2,USART1,"get t1.txt"))//对串口屏执行get t1.txt指令 用串口一发给电脑
***********************************/
void get(USART_TypeDef * USARTXx1,USART_TypeDef * USARTXx2,char *p)
{     int show;
	  zhilin(USART2,p);
	  recivehim (USART2,USART1);
	  zhilin(USART2,p);
	  for(show=0;show<200;show++)
		{
		 recivehim (USART2,USART1);
		}
}
/************************************
函数名:recivehim
描述：被动接受并处理返回的值 两个参数是用x1接受用x2发出
输入：recivehim((串口屏所在的串口，给电脑发数据的串口，相应的指令)；
输出：无
样例：get(USART2,USART1,"get t1.txt"))//对串口屏执行get t1.txt指令 用串口一发给电脑
***********************************/
//
void recivehim(USART_TypeDef * USARTx1,USART_TypeDef * USARTx2)
{     int flag;
	  char date;
	for(flag=0;flag<100;flag++)
	{
		if(USART_Get_state(USARTx1)) 
		{
		  date=USART_Get_char(USARTx1);
		  if(date!=0x70&&date!=0xff&&date!=0x1a)
			 USART_Sent_char(USARTx2,date);
		}	    
	}    
}


















