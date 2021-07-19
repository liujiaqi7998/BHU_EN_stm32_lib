/***********************************
file：led_1488.c
作者：徐烨君
时间：2019/7/18
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
#include "STM32_head.h"
#include "led_1488.h"
//直接调用led_1488_display(uint8_t dat)即可 括号中传入数据
//调用会初始化A4->DS,A5->STCP/RCLK,A6->SH_CP/SCLK
//50MHZ,推挽输出,
//************************************
#define DS A4
#define RCLK A5
#define SCLK A6

uint8_t ledduan[10][8]={
{0x00,0x00,0x3e,0x41,0x41,0x41,0x3e,0x00}, //0
{0x00,0x00,0x00,0x00,0x21,0x7f,0x01,0x00}, //1
{0x00,0x00,0x27,0x45,0x45,0x45,0x39,0x00}, //2
{0x00,0x00,0x22,0x49,0x49,0x49,0x36,0x00}, //3
{0x00,0x00,0x0c,0x14,0x24,0x7f,0x04,0x00}, //4
{0x00,0x00,0x72,0x51,0x51,0x51,0x4e,0x00}, //5
{0x00,0x00,0x3e,0x49,0x49,0x49,0x26,0x00}, //6
{0x00,0x00,0x40,0x40,0x40,0x4f,0x70,0x00}, //7
{0x00,0x00,0x36,0x49,0x49,0x49,0x36,0x00}, //8
{0x00,0x00,0x32,0x49,0x49,0x49,0x3e,0x00}, //9
};
uint8_t ledwei[8]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};//扫描编码段(列)
 

//void NOP(void)
//{
//  uint16_t i;
//  for (i = 0; i < 20; i++);
//}
extern void NOP(void);
void HC595SendData1(uint8_t lie,uint8_t hang)
{
  uint8_t i;
	gpio_Setstate(SCLK,0);//SCLK拉低
	gpio_Setstate(RCLK,0);//RCLK拉低
	 NOP();
  for (i = 0; i < 8; i++)
  {
    if (((lie << i) & 0x80) != 0 ) gpio_Setstate(DS,1);//数据线接收
    else gpio_Setstate(DS,0);
 
    gpio_Setstate(SCLK,1);//SCLK拉高
			NOP();
    gpio_Setstate(SCLK,0);//SCLK
  }
	  for (i = 0; i < 8; i++)
  {
    if (((hang<< i) & 0x80) != 0 ) gpio_Setstate(DS,1);//数据线接收
    else gpio_Setstate(DS,0);
 
    gpio_Setstate(SCLK,1);//SCLK拉高
			NOP();
    gpio_Setstate(SCLK,0);//SCLK
  }
   gpio_Setstate(RCLK,0);//RCLK
		NOP();
   gpio_Setstate(RCLK,1);//RCLK
		NOP();
   gpio_Setstate(RCLK,0);//RCLK
}
 
void led_1488_display(uint8_t dat)
{
	uint8_t n,i=0;
	gpio_init(DS,3,0);//A4,50MHZ,推挽输出,14脚串行输入DS
	gpio_init(RCLK,3,0);//A5,50MHZ,推挽输出,12脚存储寄存器时钟ST_CP
	gpio_init(SCLK,3,0);//A6,50MHZ,推挽输出,SH_CP/SCLK
	for(i=0;i<200;i++)
	{
		for(n=0;n<8;n++)
			{
				HC595SendData1(ledwei[n],ledduan[dat][n]);
				//HC595SendData1(0xfe,0xfe);
			}
	}
}
