#ifndef __QDTECH_H
#define __QDTECH_H

#include "STM32_head.h"

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//»ÒÉ«0 3165 00110 001011 00101
#define GRAY1   0x8410      	//»ÒÉ«1      00000 000000 00000
#define GRAY2   0x4208      	//»ÒÉ«2  1111111111011111

void QDtech_RS_Init(void);
void QDtech_CS_Init(void);
void QDtech_MOSI_Init(void);
void QDtech_SCL_Init(void);
void QDtech_RST_Init(void);
void QDtech_LED_Init(void);
void SPIv_WriteData(u8 Data);
void QDtech_WriteIndex(u8 Index);
void QDtech_WriteData(u8 Data);
void QDtech_WriteData_16Bit(u16 Data);
void QDtech_WriteReg(u8 Index,u16 Data);
void QDtech_Init(int left_right_side);
void QDtech_Init(int left_right_side);
void QDtech_SetXY(u16 Xpos, u16 Ypos);
void QDtech_SetRegion(u8 xStar, u8 yStar,u8 xEnd,u8 yEnd);
void QDtech_Gui_DrawPoint(u16 x,u16 y,u16 Data);
void QDtech_Lcd_Clear(u16 Color);
void QDtech_Gui_DrawStraight_Line(u16 xStar, u16 yStar,u16 xEnd, u16 yEnd,u16 Color);
void QDtech_Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);


#endif



