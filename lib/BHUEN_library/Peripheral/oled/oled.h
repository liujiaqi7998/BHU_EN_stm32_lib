#ifndef _OLED_H
#define _OLED_H
#include "STM32_head.h"

void OLED_WR_Byte(u8 date,int i);//写入一个数据字节/一个命令字节
void OLED_Refresh_Gram(void);//更新数据到ssd1306
void OLED_Display_On(void);//开启显示
void OLED_Display_Off(void);//关闭显示
void OLED_Clear(void);//清屏
void OLED_DrawPoint(u8 x,u8 y,u8 t);//画点
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//显示字符
void initoled(void);//oled 和 io 口初始化
void OLED_ShowString(u8 x,u8 y,const u8 *p);//写字符串
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowfloatNum(u8 x,u8 y,float num,u8 len,u8 floatlen,u8 size);
#endif
















