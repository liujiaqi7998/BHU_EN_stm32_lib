#ifndef __DS1302_H
#define __DS1302_H
#include "sys.h"

void DS1302_CLK_Init(void);
void DS1302_DAT_Init(void);
void DS1302_RST_Init(void);
void DS1302_DAT_Change_In(void);
void DS1302_DAT_Change_Out(void);
void DS1302_1uchar_Write(u8 dat);
void DS1302_Write(u8 com,u8 dat);
u8 DS1302_1uchar_Read(void);
u8 DS1302_Read(u8 cmd);
void DS1302_Init(void);
char * DS1302_ReadTime(void);
char * DS1302_ReadDate(void);


#endif



