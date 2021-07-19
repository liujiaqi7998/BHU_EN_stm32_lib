#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#include "STM32_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
   	   
//IO方向设置
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;}

//IO操作函数	 
//#define IIC_SCL    PBout(6) 		//SCL
//#define IIC_SDA    PBout(7) 		//SDA	 
//#define READ_SDA   PBin(7) 	 		//输入SDA 

//IIC所有操作函数
extern void IIC_Init(PIN_enum IIC_SCL,PIN_enum IIC_SDA);			 
extern void IIC_Start(PIN_enum IIC_SCL,PIN_enum IIC_SDA);				//发送IIC开始信号
extern void IIC_Stop(PIN_enum IIC_SCL,PIN_enum IIC_SDA);	  			//发送IIC停止信号
extern void IIC_Send_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,u8 txd);			//IIC发送一个字节
extern u8 IIC_Read_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,unsigned char ack);//IIC读取一个字节
extern u8 IIC_Wait_Ack(PIN_enum IIC_SCL,PIN_enum IIC_SDA); 				//IIC等待ACK信号
extern void IIC_Ack(PIN_enum IIC_SCL,PIN_enum IIC_SDA);					//IIC发送ACK信号
extern void IIC_NAck(PIN_enum IIC_SCL,PIN_enum IIC_SDA);				//IIC不发送ACK信号

extern void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















