#ifndef __LCD1602_H
#define __LCD1602_H
#include "stm32f10x.h"

#define CMD  0
#define DATA 1

#define LCD1602_Dir_Left  0x06
#define LCD1602_Dir_Right 0x04
#define LCD1602_Cur_ENAF  0x0F  	//光标显示且闪烁
#define LCD1602_Cur_ENA   0x0E  	//光标显示但不闪烁
#define LCD1602_Cur_Disa  0x0C  	//光标不显示

void LCD1602_Cls(void);
void LCD1602_Init(void);
void LCD1602_Write(uint8_t data,uint8_t cmd);
void LCD1602_WriteString(char *data);
void LCD1602_WriteChar(uint8_t data);
void LCD1602_SetCur(uint8_t row,uint8_t col);

#endif

