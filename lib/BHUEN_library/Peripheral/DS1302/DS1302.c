#include "STM32_head.h"
#include "string.h"
#include "DS1302.h"

#define DS1302_CLK A5
#define DS1302_DAT A6
#define DS1302_RST A7//时钟芯片针脚，可改

/***********************************
file：QDtech.c
作者：苏子朗
时间：2019/7/19
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
char day[7][3]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

/************************************
函数名:DS1302_CLK_Init
描述：DS1302_CLK口初始化设置
输入：无
输出：无
样例：DS1302_CLK_Init(GPIOB,11);
***********************************/
void DS1302_CLK_Init(void)
{
	gpio_init(DS1302_CLK,3,0);
}
/************************************
函数名:DS1302_DAT_Init
描述：DS1302_DAT口初始化设置
输入：无
输出：无
样例：DS1302_DAT_Init(void);//GPIOB,针脚12
***********************************/
void DS1302_DAT_Init(void)
{
	gpio_init(DS1302_DAT,3,0);
	
}
/************************************
函数名:DS1302_RST_Init
描述：DS1302_RST口初始化设置
输入：无
输出：无
样例：DS1302_RST_Init(void);
***********************************/
void DS1302_RST_Init(void)
{
	gpio_init(DS1302_RST,3,0);
}
/************************************
函数名:DS1302_DAT_Change_In
描述：DS1302_DAT设置成输入模式
输入：无
输出：无
样例：DS1302_DAT_Change_In(void);
***********************************/
void DS1302_DAT_Change_In(void)
{
	gpio_init(DS1302_DAT,0,2);
}
/************************************
函数名:DS1302_DAT_Change_Out
描述：DS1302_DAT设置成输出模式
输入：无
输出：无
样例：DS1302_DAT_Change_Out(void);
***********************************/
void DS1302_DAT_Change_Out(void)
{
	gpio_init(DS1302_DAT,3,0);
}
/************************************
函数名:DS1302_1uchar_Write
描述：向DS1302写入一个字节
输入：dat(写入的字节)
输出：无
样例：DS1302_1uchar_Write(0xff);
***********************************/
void DS1302_1uchar_Write(u8 dat)
{
	u8 i;
	DS1302_DAT_Change_Out();
	gpio_Setstate(DS1302_CLK,1);
	delay_us(2);
	for(i=0;i<8;i++)
	{
		gpio_Setstate(DS1302_DAT,dat&0x01);
		delay_us(2);
		gpio_Setstate(DS1302_CLK,1);
		delay_us(2);
		gpio_Setstate(DS1302_CLK,0);
		dat=dat>>1;
	}
}
/************************************
函数名:DS1302_Write
描述：向DS1302中的寄存器写入一个字节
输入oom(寄存器地址)dat(写入的字节)
输出：无
样例：DS1302_1uchar_Write(0x80,0xff);
***********************************/
void DS1302_Write(u8 com,u8 dat)
{
	DS1302_DAT_Change_Out();
	gpio_Setstate(DS1302_RST,0);
	gpio_Setstate(DS1302_CLK,0);
	gpio_Setstate(DS1302_RST,1);
	delay_us(2);
	DS1302_1uchar_Write(com);
	DS1302_1uchar_Write(dat);
	gpio_Setstate(DS1302_CLK,1);
	gpio_Setstate(DS1302_DAT,1);
	gpio_Setstate(DS1302_RST,0);
}
/************************************
函数名:DS1302_1uchar_Read
描述：向DS1302读取一个字节
输入：无
输出：u8的一个字节数据
样例：data=DS1302_1uchar_Write();
***********************************/
u8 DS1302_1uchar_Read(void)
{
	u8 i,dat=0x00;
	DS1302_DAT_Change_In();
	for(i=0;i<8;i++)
	{
		dat=dat>>1;
		gpio_Setstate(DS1302_CLK,0);
		if(gpio_GetIO(DS1302_DAT))
			dat=dat|0x80;
		gpio_Setstate(DS1302_CLK,1);
		delay_us(2);
	}
	return dat;
}
/************************************
函数名:DS1302_Read
描述：向DS1302中的寄存器读取一个字节
输入：cmd(寄存器地址)
输出：u8的一个字节数据
样例：data=DS1302_1uchar_Write(0x81);
***********************************/
u8 DS1302_Read(u8 cmd)
{
	u8 dat;
	DS1302_DAT_Change_Out();
	gpio_Setstate(DS1302_RST,0);
	gpio_Setstate(DS1302_CLK,0);
	gpio_Setstate(DS1302_DAT,1);
	gpio_Setstate(DS1302_RST,1);
	delay_us(2);
	DS1302_1uchar_Write(cmd);
	dat=DS1302_1uchar_Read();
	gpio_Setstate(DS1302_CLK,1);
	gpio_Setstate(DS1302_DAT,1);
	gpio_Setstate(DS1302_RST,0);
	return dat;
}
/************************************
函数名:DS1302_Init
描述：DS1302的初始化
输入：无
输出：无
样例：DS1302_Init();
***********************************/
void DS1302_Init(void)
{
	DS1302_CLK_Init();
	DS1302_DAT_Init();
	DS1302_RST_Init();
	DS1302_Write(0x8e,0x00);
  DS1302_Write(0x80,0x00);
	DS1302_Write(0x82,0x00);
	DS1302_Write(0x84,0x81);
	DS1302_Write(0x86,0x01);
	DS1302_Write(0x88,0x01);
	DS1302_Write(0x8a,0x07);
	DS1302_Write(0x8c,0x00);
	DS1302_Write(0x8e,0x80);
} 
/************************************
函数名:DS1302_ReadTime
描述：读取DS1302的时间
输入：无
输出：一个字符串的指针,指针指向含有时间的字符串
样例：char* time
time=DS1302_ReadTime();
***********************************/
char * DS1302_ReadTime(void)
{
	u8 shi,fen,miao,t;
	char shijian[10]={' '};
	static char final[10];
	shi=DS1302_Read(0x85);
	fen=DS1302_Read(0x83);
	miao=DS1302_Read(0x81);
		if((shi&0x80)==0x80)
	{
		shijian[1]='M';
		if((shi&0x20)==0x20)
			shijian[0]='P';
		else
			shijian[0]='A';
		t=shi;
		t=t&0x0f;
		shijian[3]=t+'0';
		if((shi&0x10)==0x10)
			shijian[2]='1';
		else
			shijian[2]='0';
	}
	else
	{
		shijian[0]=' ';
		shijian[1]=' ';
		t=shi;
		t=t&0x0f;
		shijian[3]=t+'0';
		t=shi;
		t=t&0x30;
		t>>=4;
		shijian[2]=t+'0';
	}
	shijian[4]=':';
	t=fen;
	t=t&0x0f;
	shijian[6]=t+'0';
	t=fen;
	t=t&0x70;
	t>>=4;
	shijian[5]=t+'0';
	shijian[7]=':';
	t=miao;
	t=t&0x0f;
	shijian[9]=t+'0';
	t=miao;
	t=t&0x70;
	t>>=4;
	shijian[8]=t+'0';
	strcpy(final,shijian);
	return final;
}
/************************************
函数名:DS1302_ReadDate
描述：读取DS1302的时间
输入：无
输出：一个字符串的指针,指针指向含有日期的字符串
样例：char* date
date=DS1302_ReadDate();
***********************************/
char * DS1302_ReadDate(void)
{
	u8 nian=DS1302_Read(0x8d),yue=DS1302_Read(0x89),ri=DS1302_Read(0x87);
	int xing;
	u8 month,day_before,year;
	char date[11]={' '};
	static char final[14];
	u8 t;
	year=0;
	date[0]='2';
	date[1]='0';
	t=nian;
	t=t&0x0f;
	date[3]=t+'0';
	year+=t;
	t=nian;
	t=t&0xf0;
	t>>=4;
	date[2]=t+'0';
	year+=t*10;
	date[4]='-';
	month=0;
	t=yue;
	t=t&0x0f;
	month+=t;
	date[6]=t+'0';
	if((yue&0x10)==0x10)
	{
		date[5]='1';
		month+=10;
	}
	else
		date[5]='0';
	date[7]='-';
	day_before=0;
	t=ri;
	t=t&0x0f;
	date[9]=t+'0';
	day_before+=t;
	t=ri;
	t=t&0x30;
	t>>=4;
	date[8]=t+'0';
	day_before+=t*10;
	switch(month)
	{
		case 1:
		{
			if(year%400==0||(year%4==0&&year%100!=0))
				month=5;
			else
				month=6;
			break;
		}
		case 2:
		{
			if(year%400==0||(year%4==0&&year%100!=0))
				month=1;
			else
				month=2;
			break;
		}
		case 10:month=6;break;
		case 3:
		case 11:month=2;break;
		case 4:
		case 7:month=5;break;
		case 5:month=0;break;
		case 6:month=3;break;
		case 8:month=1;break;
		case 9:
		case 12:month=4;break;
	}
	xing=(year/4+year%7+month+day_before)%7;
	if(xing<0)
		xing=(xing%7+7)%7;
	DS1302_Write(0x8e,0x00);
	DS1302_Write(0x8a,xing+1);
	DS1302_Write(0x8e,0x80);
	strcpy(final,date);
	strcat(final,day[xing]);
	return final;
}
