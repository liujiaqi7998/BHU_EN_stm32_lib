#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#include "STM32_gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
   	   
//IO��������
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;}

//IO��������	 
//#define IIC_SCL    PBout(6) 		//SCL
//#define IIC_SDA    PBout(7) 		//SDA	 
//#define READ_SDA   PBin(7) 	 		//����SDA 

//IIC���в�������
extern void IIC_Init(PIN_enum IIC_SCL,PIN_enum IIC_SDA);			 
extern void IIC_Start(PIN_enum IIC_SCL,PIN_enum IIC_SDA);				//����IIC��ʼ�ź�
extern void IIC_Stop(PIN_enum IIC_SCL,PIN_enum IIC_SDA);	  			//����IICֹͣ�ź�
extern void IIC_Send_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,u8 txd);			//IIC����һ���ֽ�
extern u8 IIC_Read_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,unsigned char ack);//IIC��ȡһ���ֽ�
extern u8 IIC_Wait_Ack(PIN_enum IIC_SCL,PIN_enum IIC_SDA); 				//IIC�ȴ�ACK�ź�
extern void IIC_Ack(PIN_enum IIC_SCL,PIN_enum IIC_SDA);					//IIC����ACK�ź�
extern void IIC_NAck(PIN_enum IIC_SCL,PIN_enum IIC_SDA);				//IIC������ACK�ź�

extern void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
extern u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















