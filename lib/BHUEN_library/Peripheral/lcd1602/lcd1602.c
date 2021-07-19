#include "STM32_head.h"
#include "lcd1602.h"
/***********************************
file：lcd1602.c
作者：杨帆
时间：2019/7/20
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
#define High  0xFF00
#define Low   0x00FF
#define Pin_Data Low  	  //数据线为低8位
#define PIN_Low
/***********************************
#define Pin_Data Low
#define Pin_Low  时使用低8脚为数据传输引脚

#define Pin_Data High
#define Pin_High  时使用高8脚为数据传输引脚
***********************************/
#define RS    A8
#define RW    A9
#define RE    A10
#define GPIOx GPIOA
//在此修改使用的引脚
static uint8_t nrow=0,ncol=0;
/************************************
函数名:LCD1602_Init
描述： lcd初始化
输入： 无
输出： 无
样例： LCD1602_Init();
***********************************/
void LCD1602_Init()
{
	GPIO_init_All_like(GPIOx,0,3,Pin_Data);
	gpio_init(RW,0,3);
	gpio_init(RS,0,3);
	gpio_init(RE,0,3);
	
	GPIO_Setstate(GPIOx,RE,0);
	
	LCD1602_Write(0x38,CMD);
	LCD1602_Write(0x0C,CMD);
	LCD1602_Write(0x06,CMD);
	LCD1602_Write(0x01,CMD);
	delay_ms(100);
}
/************************************
(系统自动调用，用户不需要查看)
函数名:LCD1602_Write
描述： 向lcd写入数据或命令
输入： data: 要输入的数据或命令
	   cmd : CMD(输入命令), DATA(输入数据)
输出： 
样例： LCD1602_Write(0x01,Cmd);//清屏
***********************************/
void LCD1602_Write(uint8_t data,uint8_t cmd)
{
	if(cmd)
		gpio_Setstate(RS,1);
	else
		gpio_Setstate(RS,0);
	gpio_Setstate(RW,0);
#ifdef PIN_Low
		GPIOx->ODR &= 0xFF00;
		GPIOx->ODR |= data;
#endif
#ifdef PIN_High
		GPIOx->ODR &= 0x00FF;
		GPIOx->ODR |= data<<8;
#endif
	delay_ms(1);
	gpio_Setstate(RE,1);
	delay_ms(1);
	gpio_Setstate(RE,0);
}
/************************************
函数名:LCD1602_SetCur
描述： 设置游标位置
输入： row 行,col 列
输出： 无
样例： LCD1602_SetCur(0,0);//游标设置到第0行第0列
***********************************/
void LCD1602_SetCur(uint8_t row,uint8_t col)
{
	nrow = row;
	ncol = col;
	LCD1602_Write(0x80+col+0x40*row,CMD);
}
/************************************
函数名:LCD1602_WriteChar
描述： 写入单个字符
输入： data要写入的字符
输出： 无
样例： LCD1602_WriteChar('A');//显示A
***********************************/
void LCD1602_WriteChar(uint8_t data)
{	
	LCD1602_Write(data,DATA);
	if(ncol == 15)
	{
		nrow = !nrow;
		ncol = 0;
		LCD1602_SetCur(nrow,ncol);
	}
	else
	{
		ncol++;
	}
}
/************************************
函数名:LCD1602_WriteString
描述： 写入字符串
输入： *data字符串地址
输出： 无
样例： LCD1602_WriteString(":(");//显示:(
***********************************/
void LCD1602_WriteString(char *data)
{
	while(*data != '\0')
	{
		LCD1602_Write(*data,DATA);
		data++;
		if(ncol == 15)
		{
			nrow = !nrow;
			ncol = 0;
			LCD1602_SetCur(nrow,ncol);
		}
		else
		{
			ncol++;
		}
	}
}
/************************************
函数名:LCD1602_Cls
描述： 清屏
输入： 无
输出： 无
样例： LCD1602_Cls();
***********************************/
void LCD1602_Cls()
{
	LCD1602_Write(0x01,CMD);
	delay_ms(50);
}
/************************************
函数名:LCD1602_Config
描述： 设置游标状态和位移方向
输入： Dir: 输入LCD1602_Dir_Right(向右位移)
	   	    输入LCD1602_Dir_Left (向左位移)
	   Cur: LCD1602_Cur_ENA (显示游标)
		    LCD1602_Cur_ENAF(显示游标且游标闪烁)
		    LCD1602_Cur_Dis (不显示游标)	   
输出： 无
样例： LCD1602_WriteByte(LCD1602_Dir_Right,LCD1602_Cur_ENAF);//游标显示且闪烁，向右位移
***********************************/
void LCD1602_Config(uint8_t dir,uint8_t cur)
{
	LCD1602_Write(dir,CMD);
	LCD1602_Write(cur,CMD);
}

