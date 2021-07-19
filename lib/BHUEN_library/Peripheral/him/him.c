#include "him.h"
#include "STM32_head.h"
/***********************************
file��him.c
���ߣ��Ű�(skr)
ʱ�䣺2019/7/18
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
//			֧�ִ��ڣ�
//      USART1_TX ------ PA9       //
//      USART1_RX ----- PA10       //
//      USART2_TX ----- PA2        //
//      USART2_RX ----- PA3        //
//      USART3_TX ----- PB10       //
//      USART3_RX ----- PB11       //
/************************************
������:init_HMI
��������������ʼ��
���룺USARTx(����)
�������
������init_HMI(USART1);//USART1�����ϵĴ�������ʼ��
***********************************/
void init_HMI(USART_TypeDef * USARTX)
{
	delay_ms(200);
	USART_Sent_char(USARTX,0xff);
	delay_ms(200);	delay_ms(200);
	delay_ms(200);

}
/************************************
������:clear
����������������
���룺USARTx(����)
�������
������init_HMI(USART1);//USART1�����ϵĴ���������
***********************************/
void clear(USART_TypeDef * USARTX)
{   
    char p[10]="page 0";
    zhilin(USARTX,p);
}
/************************************
������:zhilin
��������������ָ��
���룺USARTx(����)
�������
������zhilin(USART1,"t1.txt"))//��ȡ����һ�ϵĴ�����t1.txt
***********************************/
void zhilin(USART_TypeDef * USARTX,char *p)//��������дָ��
{
	        USART_Sent_String(USARTX,p,0);
	        USART_Sent_char(USARTX,0xff);
		    USART_Sent_char(USARTX,0xff);
	        USART_Sent_char(USARTX,0xff);
}
/************************************
������:display
��������һЩ�����ڴ�����������ʾ
���룺chose(ѡ��Ҫ������������0Ϊ����1Ϊ�ַ�2Ϊ���ּ��ַ�)��USARTx(���ڣ�Ҫִ�е�ָ��ַ���������)��
�������
������display(1��USART1,"t1.txt"����2����p))//���ַ���2��t1.txt
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
������:numchuan
��������Ҫ����������������һЩ����
���룺numchuan((���ڣ���������)��
�������
������display(USART1,p))//��p�����е�ÿ��Ԫ�ؽ��в�ִ���
***********************************/
void numchuan(USART_TypeDef * USARTX,int p)
{   
	int i;//ѭ�������õ���
	int k;//��¼�����Ǽ�λ��
	int num[100];//��һ������洢��������
	int flag;//������������
	int i1;//forѭ��ʹ��
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
������:get
��������ȡ��Ļ���� ��Ҫʲô���ݾ�get ɶ ���� get t1.txt
���룺get((���������ڵĴ��ڣ������Է����ݵĴ��ڣ���Ӧ��ָ��)��
�������
������get(USART2,USART1,"get t1.txt"))//�Դ�����ִ��get t1.txtָ�� �ô���һ��������
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
������:recivehim
�������������ܲ������ص�ֵ ������������x1������x2����
���룺recivehim((���������ڵĴ��ڣ������Է����ݵĴ��ڣ���Ӧ��ָ��)��
�������
������get(USART2,USART1,"get t1.txt"))//�Դ�����ִ��get t1.txtָ�� �ô���һ��������
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


















