/************************************
file：GY.c
作者：陈捷
时间：2019/7/17
本代码仅供北华大学嵌入式系统实验室使用
************************************/
#include "GY.h"
#include "STM32_head.h"

#define IIC_SCL  B6
#define IIC_SDA  B7
/************************************
函数名: GY_Get_Gyroscope
描述: 得到陀螺仪值(原始值)
输入: gx,gy,gz:陀螺仪 x,y,z 轴的原始读数(带符号)
输出: 0,成功 其他,错误代码
样例: GY_Get_Gyroscope(&gx,&gy,&gz);
************************************/
u8 GY_Get_Gyroscope(short *gx,short *gy,short *gz)
{
	u8 buf[6],res;
	res=GY_Read_Len(GY_ADDR,GY_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];
		*gy=((u16)buf[2]<<8)|buf[3];
		*gz=((u16)buf[4]<<8)|buf[5];
	}
	return res;
}
/************************************
函数名: GY_Get_Accelerometer
描述: 得到加速度值(原始值)
输入: gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
输出: 0,成功 其他,错误代码
样例: GY_Get_Accelerometer(&ax,&ay,&az);
************************************/
u8 GY_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=GY_Read_Len(GY_ADDR,GY_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
/************************************
函数名: GY_Write_Byte
描述: IIC写一个字节
输入: reg:寄存器地址 data:数据
输出: 0,正常 其他,错误代码
样例: GY_Write_Byte(GY_PWR_MGMT1_REG,0X80);
************************************/
u8 GY_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(IIC_SCL,IIC_SDA); 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(GY_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))	//等待应答
	{
		IIC_Stop(IIC_SCL,IIC_SDA);		 
		return 1;		
	}
    IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);	//写寄存器地址
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//等待应答 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,data);//发送数据
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))	//等待ACK
	{
		IIC_Stop(IIC_SCL,IIC_SDA);	 
		return 1;		 
	}		 
    IIC_Stop(IIC_SCL,IIC_SDA);	 
	return 0;
}
/************************************
函数名: GY_Read_Byte
描述: IIC读一个字节 
输入: reg:寄存器地址
输出: 读到的数据
样例: GY_Read_Byte(GY_PWR_MGMT1_REG);
************************************/
u8 GY_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(IIC_SCL,IIC_SDA); 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(GY_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//等待应答 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);	//写寄存器地址
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//等待应答
    IIC_Start(IIC_SCL,IIC_SDA);
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(GY_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//等待应答 
	res=IIC_Read_Byte(IIC_SCL,IIC_SDA,0);//读取数据,发送nACK 
    IIC_Stop(IIC_SCL,IIC_SDA);			//产生一个停止条件 
	return res;		
}
/************************************
函数名: GY_Write_Len
描述: IIC连续写
输入: addr:器件地址 reg:寄存器地址 len:写入长度 buf:数据区
输出: 0,正常 其他,错误代码
样例: GY_Read_Len(GY_ADDR,GY_ACCEL_XOUTH_REG,6,buf);
************************************/
u8 GY_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(IIC_SCL,IIC_SDA); 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(addr<<1)|0);//发送器件地址+写命令	
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))	//等待应答
	{
		IIC_Stop(IIC_SCL,IIC_SDA);		 
		return 1;		
	}
    IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);	//写寄存器地址
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(IIC_SCL,IIC_SDA,buf[i]);	//发送数据
		if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))		//等待ACK
		{
			IIC_Stop(IIC_SCL,IIC_SDA);	 
			return 1;		 
		}		
	}    
    IIC_Stop(IIC_SCL,IIC_SDA);	 
	return 0;	
}
/************************************
函数名: GY_Read_Len
描述: IIC连续读
输入: addr:器件地址 reg:要读取的寄存器地址 len:要读取的长度 buf:读取到的数据存储区
输出: 0,正常 其他,错误代码
样例: GY_Read_Len(GY_ADDR,GY_GYRO_XOUTH_REG,6,buf);
************************************/
u8 GY_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	IIC_Start(IIC_SCL,IIC_SDA);
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(addr<<1)|0); //发送器件地址+写命令 
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA)) //等待应答
	{ 
		IIC_Stop(IIC_SCL,IIC_SDA);return 1; 
	}
	IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);  //写寄存器地址
	IIC_Wait_Ack(IIC_SCL,IIC_SDA);  //等待应答
	IIC_Start(IIC_SCL,IIC_SDA);
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(addr<<1)|1);//发送器件地址+读命令 
	IIC_Wait_Ack(IIC_SCL,IIC_SDA);  //等待应答
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(IIC_SCL,IIC_SDA,0);//读数据,发送 nACK
		else *buf=IIC_Read_Byte(IIC_SCL,IIC_SDA,1); //读数据,发送 ACK
		len--; buf++;
	}
	IIC_Stop(IIC_SCL,IIC_SDA);  //产生一个停止条件
	return 0;
}
/************************************
函数名: GY_Set_Gyro_Fsr
描述: 设置MPU6050陀螺仪传感器满量程范围
输入: fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
输出: 0,设置成功 其他,设置失败 
样例: GY_Set_Gyro_Fsr(3);
************************************/
u8 GY_Set_Gyro_Fsr(u8 fsr)
{
	return GY_Write_Byte(GY_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
/************************************
函数名: GY_Set_Accel_Fsr
描述: 设置MPU6050加速度传感器满量程范围
输入: fsr:0,±2g;1,±4g;2,±8g;3,±16g
输出: 0,设置成功 其他,设置失败 
样例: GY_Set_Accel_Fsr(0);
************************************/
u8 GY_Set_Accel_Fsr(u8 fsr)
{
	return GY_Write_Byte(GY_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
/************************************
函数名: GY_Set_LPF
描述: 设置MPU6050的数字低通滤波器
输入: lpf:数字低通滤波频率(Hz)
输出: 0,设置成功 其他,设置失败 
样例: GY_Set_LPF(10);
************************************/
u8 GY_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return GY_Write_Byte(GY_CFG_REG,data);//设置数字低通滤波器  
}
/************************************
函数名: GY_Set_Rate
描述: 设置MPU6050的采样率(假定Fs=1KHz)
输入: rate:4~1000(Hz)
输出: 0,设置成功 其他,设置失败 
样例: GY_Set_Rate(50);
************************************/
u8 GY_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=GY_Write_Byte(GY_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return GY_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}
/************************************
函数名: GY_Init
描述: 陀螺仪初始化
输入: 无
输出: 无
样例: GY_Init();
************************************/
u8 GY_Init()
{
	u8 res;
	RCC->APB2ENR|=1<<2;
	GPIOA->CRL&=0XFFFFFFF0;
	GPIOA->CRL|=0X00000003;
	GY_AD0_CTRL=0;
	
	IIC_Init(IIC_SCL,IIC_SDA);//初始化IIC总线
	GY_Write_Byte(GY_PWR_MGMT1_REG,0X80);	//复位MPU6050
    delay_ms(100);
	GY_Write_Byte(GY_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	GY_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	GY_Set_Accel_Fsr(0);					//加速度传感器,±2g
	GY_Set_Rate(50);						//设置采样率50Hz
	GY_Write_Byte(GY_INT_EN_REG,0X00);	//关闭所有中断
	GY_Write_Byte(GY_USER_CTRL_REG,0X00);	//I2C主模式关闭
	GY_Write_Byte(GY_FIFO_EN_REG,0X00);	//关闭FIFO
	GY_Write_Byte(GY_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=GY_Read_Byte(GY_DEVICE_ID_REG);
	if(res==GY_ADDR)//器件ID正确
	{
		GY_Write_Byte(GY_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		GY_Write_Byte(GY_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		GY_Set_Rate(50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}

