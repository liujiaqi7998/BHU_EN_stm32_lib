#include "STM32_head.h"
#include "lcd1602.h"
/***********************************
file��lcd1602.c
���ߣ��
ʱ�䣺2019/7/20
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
#define High  0xFF00
#define Low   0x00FF
#define Pin_Data Low  	  //������Ϊ��8λ
#define PIN_Low
/***********************************
#define Pin_Data Low
#define Pin_Low  ʱʹ�õ�8��Ϊ���ݴ�������

#define Pin_Data High
#define Pin_High  ʱʹ�ø�8��Ϊ���ݴ�������
***********************************/
#define RS    A8
#define RW    A9
#define RE    A10
#define GPIOx GPIOA
//�ڴ��޸�ʹ�õ�����
static uint8_t nrow=0,ncol=0;
/************************************
������:LCD1602_Init
������ lcd��ʼ��
���룺 ��
����� ��
������ LCD1602_Init();
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
(ϵͳ�Զ����ã��û�����Ҫ�鿴)
������:LCD1602_Write
������ ��lcdд�����ݻ�����
���룺 data: Ҫ��������ݻ�����
	   cmd : CMD(��������), DATA(��������)
����� 
������ LCD1602_Write(0x01,Cmd);//����
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
������:LCD1602_SetCur
������ �����α�λ��
���룺 row ��,col ��
����� ��
������ LCD1602_SetCur(0,0);//�α����õ���0�е�0��
***********************************/
void LCD1602_SetCur(uint8_t row,uint8_t col)
{
	nrow = row;
	ncol = col;
	LCD1602_Write(0x80+col+0x40*row,CMD);
}
/************************************
������:LCD1602_WriteChar
������ д�뵥���ַ�
���룺 dataҪд����ַ�
����� ��
������ LCD1602_WriteChar('A');//��ʾA
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
������:LCD1602_WriteString
������ д���ַ���
���룺 *data�ַ�����ַ
����� ��
������ LCD1602_WriteString(":(");//��ʾ:(
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
������:LCD1602_Cls
������ ����
���룺 ��
����� ��
������ LCD1602_Cls();
***********************************/
void LCD1602_Cls()
{
	LCD1602_Write(0x01,CMD);
	delay_ms(50);
}
/************************************
������:LCD1602_Config
������ �����α�״̬��λ�Ʒ���
���룺 Dir: ����LCD1602_Dir_Right(����λ��)
	   	    ����LCD1602_Dir_Left (����λ��)
	   Cur: LCD1602_Cur_ENA (��ʾ�α�)
		    LCD1602_Cur_ENAF(��ʾ�α����α���˸)
		    LCD1602_Cur_Dis (����ʾ�α�)	   
����� ��
������ LCD1602_WriteByte(LCD1602_Dir_Right,LCD1602_Cur_ENAF);//�α���ʾ����˸������λ��
***********************************/
void LCD1602_Config(uint8_t dir,uint8_t cur)
{
	LCD1602_Write(dir,CMD);
	LCD1602_Write(cur,CMD);
}

