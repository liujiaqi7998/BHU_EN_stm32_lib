/************************************
file：myiic.c
作者：陈捷
时间：2019/7/17
本代码仅供北华大学嵌入式系统实验室使用
************************************/
#include "myiic.h"
#include "STM32_head.h"



/************************************
函数名: IIC_Init
描述: 初始化IIC
输入: 无
输出: 无
样例: IIC_Init();
************************************/
void IIC_Init(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{					     
 	//RCC->APB2ENR|=1<<3;		//先使能外设IO PORTB时钟 							 
	//GPIOB->CRL&=0X00FFFFFF;	
	//GPIOB->CRL|=0X33000000;	  //PB6/7 推挽输出
	//GPIOB->ODR|=3<<6;     	//PB6,7 输出高
	
	gpio_init(IIC_SCL,3,0);
	gpio_init(IIC_SDA,3,0);
}
/************************************
函数名: IIC_Start
描述: 产生IIC起始信号
输入: 无
输出: 无
样例: IIC_Start();
************************************/
void IIC_Start(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	//SDA_OUT();     //sda线输出
	gpio_Set(IIC_SDA,3,0);
	//IIC_SDA=1;
	gpio_Setstate(IIC_SDA,1);	  	  
	//IIC_SCL=1;
	gpio_Setstate(IIC_SCL,1);
	delay_us(4);
 	//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	gpio_Setstate(IIC_SDA,0);	  
	delay_us(4);
	//IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
	gpio_Setstate(IIC_SCL,0); 
}	
/************************************
函数名: IIC_Stop
描述: 产生IIC停止信号
输入: 无
输出: 无
样例: IIC_Stop();
************************************/
void IIC_Stop(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	//SDA_OUT();//sda线输出
	gpio_Set(IIC_SDA,3,0);
	//IIC_SCL=0;
	gpio_Setstate(IIC_SCL,0); 
	//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	gpio_Setstate(IIC_SDA,0);
 	delay_us(4);
	//IIC_SCL=1; 
	gpio_Setstate(IIC_SCL,1);
	//IIC_SDA=1;//发送I2C总线结束信号
	gpio_Setstate(IIC_SDA,1);
	delay_us(4);							   	
}
/************************************
函数名: IIC_Wait_Ack
描述: 等待应答信号到来
输入: 无
输出: 1，接收应答失败 0，接收应答成功
样例: IIC_Wait_Ack();
************************************/
u8 IIC_Wait_Ack(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	u8 ucErrTime=0;
	//SDA_IN();      //SDA设置为输入  
	//IIC_SDA=1;
	gpio_Set(IIC_SDA,0,2);
	delay_us(1);	   
	//IIC_SCL=1;
	gpio_Setstate(IIC_SCL,1);
	delay_us(1);	 
	//while(READ_SDA)
	while(gpio_GetIO(IIC_SDA))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(IIC_SCL,IIC_SDA);
			return 1;
		}
	}
	//IIC_SCL=0;//时钟输出0
	gpio_Setstate(IIC_SCL,0);	   
	return 0;  
} 
/************************************
函数名: IIC_Ack
描述: 产生ACK应答
输入: 无
输出: 无
样例: IIC_Ack();
************************************/
void IIC_Ack(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	//IIC_SCL=0;
	gpio_Setstate(IIC_SCL,0);
	//SDA_OUT();
	gpio_Set(IIC_SDA,3,0);
	//IIC_SDA=0;
	gpio_Setstate(IIC_SDA,0);
	delay_us(2);
	//IIC_SCL=1;
	gpio_Setstate(IIC_SCL,1);
	delay_us(2);
	//IIC_SCL=0;
	gpio_Setstate(IIC_SCL,0);
}
/************************************
函数名: IIC_NAck
描述: 不产生ACK应答
输入: 无
输出: 无
样例: IIC_NAck();
************************************/	    
void IIC_NAck(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	//IIC_SCL=0;
	gpio_Setstate(IIC_SCL,0);
	//SDA_OUT();
	gpio_Set(IIC_SDA,3,0);
	//IIC_SDA=1;
	gpio_Setstate(IIC_SDA,1);
	delay_us(2);
	//IIC_SCL=1;
	gpio_Setstate(IIC_SCL,1);
	delay_us(2);
	//IIC_SCL=0;
	gpio_Setstate(IIC_SCL,0);
}	
/************************************
函数名: IIC_Send_Byte
描述: IIC发送一个字节
输入: txd:一个字节
输出: 1，有应答 0，无应答
样例: IIC_Send_Byte(txd);
************************************/
void IIC_Send_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,u8 txd)
{                        
    u8 t;   
	//SDA_OUT(); 	
	gpio_Set(IIC_SDA,3,0);    
    //IIC_SCL=0;//拉低时钟开始数据传输
    gpio_Setstate(IIC_SCL,0);
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
        gpio_Setstate(IIC_SDA,(txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		//IIC_SCL=1;
		gpio_Setstate(IIC_SCL,1);
		delay_us(2); 
		//IIC_SCL=0;
		gpio_Setstate(IIC_SCL,0);	
		delay_us(2);
    }	 
} 	  
/************************************
函数名: IIC_Read_Byte
描述: IIC读取1个字节
输入: ack: ack=1时，发送ACK，ack=0，发送nACK   
输出: 返回值
样例: IIC_Read_Byte(ack);
************************************/
u8 IIC_Read_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,unsigned char ack)
{
	unsigned char i,receive=0;
	//SDA_IN();//SDA设置为输入
	gpio_Set(IIC_SDA,0,2);
  for(i=0;i<8;i++)
	{
        //IIC_SCL=0; 
        gpio_Setstate(IIC_SCL,0);
        delay_us(2);
		//IIC_SCL=1;
		gpio_Setstate(IIC_SCL,1);
        receive<<=1;
        //if(READ_SDA)receive++;  
		if(gpio_GetIO(IIC_SDA))receive++;
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck(IIC_SCL,IIC_SDA);//发送nACK
    else
        IIC_Ack(IIC_SCL,IIC_SDA); //发送ACK   
    return receive;
}



























