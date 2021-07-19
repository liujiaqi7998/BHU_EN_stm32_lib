#ifndef _OLED_H
#define _OLED_H
#include "STM32_head.h"

void OLED_WR_Byte(u8 date,int i);//д��һ�������ֽ�/һ�������ֽ�
void OLED_Refresh_Gram(void);//�������ݵ�ssd1306
void OLED_Display_On(void);//������ʾ
void OLED_Display_Off(void);//�ر���ʾ
void OLED_Clear(void);//����
void OLED_DrawPoint(u8 x,u8 y,u8 t);//����
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//��ʾ�ַ�
void initoled(void);//oled �� io �ڳ�ʼ��
void OLED_ShowString(u8 x,u8 y,const u8 *p);//д�ַ���
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowfloatNum(u8 x,u8 y,float num,u8 len,u8 floatlen,u8 size);
#endif
















