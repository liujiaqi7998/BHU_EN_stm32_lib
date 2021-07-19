/************************************
file��GY.c
���ߣ��½�
ʱ�䣺2019/7/17
���������������ѧǶ��ʽϵͳʵ����ʹ��
************************************/
#include "GY.h"
#include "STM32_head.h"

#define IIC_SCL  B6
#define IIC_SDA  B7
/************************************
������: GY_Get_Gyroscope
����: �õ�������ֵ(ԭʼֵ)
����: gx,gy,gz:������ x,y,z ���ԭʼ����(������)
���: 0,�ɹ� ����,�������
����: GY_Get_Gyroscope(&gx,&gy,&gz);
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
������: GY_Get_Accelerometer
����: �õ����ٶ�ֵ(ԭʼֵ)
����: gx,gy,gz:������x,y,z���ԭʼ����(������)
���: 0,�ɹ� ����,�������
����: GY_Get_Accelerometer(&ax,&ay,&az);
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
������: GY_Write_Byte
����: IICдһ���ֽ�
����: reg:�Ĵ�����ַ data:����
���: 0,���� ����,�������
����: GY_Write_Byte(GY_PWR_MGMT1_REG,0X80);
************************************/
u8 GY_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(IIC_SCL,IIC_SDA); 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(GY_ADDR<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))	//�ȴ�Ӧ��
	{
		IIC_Stop(IIC_SCL,IIC_SDA);		 
		return 1;		
	}
    IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//�ȴ�Ӧ�� 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,data);//��������
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))	//�ȴ�ACK
	{
		IIC_Stop(IIC_SCL,IIC_SDA);	 
		return 1;		 
	}		 
    IIC_Stop(IIC_SCL,IIC_SDA);	 
	return 0;
}
/************************************
������: GY_Read_Byte
����: IIC��һ���ֽ� 
����: reg:�Ĵ�����ַ
���: ����������
����: GY_Read_Byte(GY_PWR_MGMT1_REG);
************************************/
u8 GY_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(IIC_SCL,IIC_SDA); 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(GY_ADDR<<1)|0);//����������ַ+д����	
	IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//�ȴ�Ӧ�� 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//�ȴ�Ӧ��
    IIC_Start(IIC_SCL,IIC_SDA);
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(GY_ADDR<<1)|1);//����������ַ+������	
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//�ȴ�Ӧ�� 
	res=IIC_Read_Byte(IIC_SCL,IIC_SDA,0);//��ȡ����,����nACK 
    IIC_Stop(IIC_SCL,IIC_SDA);			//����һ��ֹͣ���� 
	return res;		
}
/************************************
������: GY_Write_Len
����: IIC����д
����: addr:������ַ reg:�Ĵ�����ַ len:д�볤�� buf:������
���: 0,���� ����,�������
����: GY_Read_Len(GY_ADDR,GY_ACCEL_XOUTH_REG,6,buf);
************************************/
u8 GY_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(IIC_SCL,IIC_SDA); 
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(addr<<1)|0);//����������ַ+д����	
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))	//�ȴ�Ӧ��
	{
		IIC_Stop(IIC_SCL,IIC_SDA);		 
		return 1;		
	}
    IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);	//д�Ĵ�����ַ
    IIC_Wait_Ack(IIC_SCL,IIC_SDA);		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(IIC_SCL,IIC_SDA,buf[i]);	//��������
		if(IIC_Wait_Ack(IIC_SCL,IIC_SDA))		//�ȴ�ACK
		{
			IIC_Stop(IIC_SCL,IIC_SDA);	 
			return 1;		 
		}		
	}    
    IIC_Stop(IIC_SCL,IIC_SDA);	 
	return 0;	
}
/************************************
������: GY_Read_Len
����: IIC������
����: addr:������ַ reg:Ҫ��ȡ�ļĴ�����ַ len:Ҫ��ȡ�ĳ��� buf:��ȡ�������ݴ洢��
���: 0,���� ����,�������
����: GY_Read_Len(GY_ADDR,GY_GYRO_XOUTH_REG,6,buf);
************************************/
u8 GY_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	IIC_Start(IIC_SCL,IIC_SDA);
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(addr<<1)|0); //����������ַ+д���� 
	if(IIC_Wait_Ack(IIC_SCL,IIC_SDA)) //�ȴ�Ӧ��
	{ 
		IIC_Stop(IIC_SCL,IIC_SDA);return 1; 
	}
	IIC_Send_Byte(IIC_SCL,IIC_SDA,reg);  //д�Ĵ�����ַ
	IIC_Wait_Ack(IIC_SCL,IIC_SDA);  //�ȴ�Ӧ��
	IIC_Start(IIC_SCL,IIC_SDA);
	IIC_Send_Byte(IIC_SCL,IIC_SDA,(addr<<1)|1);//����������ַ+������ 
	IIC_Wait_Ack(IIC_SCL,IIC_SDA);  //�ȴ�Ӧ��
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(IIC_SCL,IIC_SDA,0);//������,���� nACK
		else *buf=IIC_Read_Byte(IIC_SCL,IIC_SDA,1); //������,���� ACK
		len--; buf++;
	}
	IIC_Stop(IIC_SCL,IIC_SDA);  //����һ��ֹͣ����
	return 0;
}
/************************************
������: GY_Set_Gyro_Fsr
����: ����MPU6050�����Ǵ����������̷�Χ
����: fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
���: 0,���óɹ� ����,����ʧ�� 
����: GY_Set_Gyro_Fsr(3);
************************************/
u8 GY_Set_Gyro_Fsr(u8 fsr)
{
	return GY_Write_Byte(GY_GYRO_CFG_REG,fsr<<3);//���������������̷�Χ  
}
/************************************
������: GY_Set_Accel_Fsr
����: ����MPU6050���ٶȴ����������̷�Χ
����: fsr:0,��2g;1,��4g;2,��8g;3,��16g
���: 0,���óɹ� ����,����ʧ�� 
����: GY_Set_Accel_Fsr(0);
************************************/
u8 GY_Set_Accel_Fsr(u8 fsr)
{
	return GY_Write_Byte(GY_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
/************************************
������: GY_Set_LPF
����: ����MPU6050�����ֵ�ͨ�˲���
����: lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
���: 0,���óɹ� ����,����ʧ�� 
����: GY_Set_LPF(10);
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
	return GY_Write_Byte(GY_CFG_REG,data);//�������ֵ�ͨ�˲���  
}
/************************************
������: GY_Set_Rate
����: ����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
����: rate:4~1000(Hz)
���: 0,���óɹ� ����,����ʧ�� 
����: GY_Set_Rate(50);
************************************/
u8 GY_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=GY_Write_Byte(GY_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return GY_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}
/************************************
������: GY_Init
����: �����ǳ�ʼ��
����: ��
���: ��
����: GY_Init();
************************************/
u8 GY_Init()
{
	u8 res;
	RCC->APB2ENR|=1<<2;
	GPIOA->CRL&=0XFFFFFFF0;
	GPIOA->CRL|=0X00000003;
	GY_AD0_CTRL=0;
	
	IIC_Init(IIC_SCL,IIC_SDA);//��ʼ��IIC����
	GY_Write_Byte(GY_PWR_MGMT1_REG,0X80);	//��λMPU6050
    delay_ms(100);
	GY_Write_Byte(GY_PWR_MGMT1_REG,0X00);	//����MPU6050 
	GY_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	GY_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
	GY_Set_Rate(50);						//���ò�����50Hz
	GY_Write_Byte(GY_INT_EN_REG,0X00);	//�ر������ж�
	GY_Write_Byte(GY_USER_CTRL_REG,0X00);	//I2C��ģʽ�ر�
	GY_Write_Byte(GY_FIFO_EN_REG,0X00);	//�ر�FIFO
	GY_Write_Byte(GY_INTBP_CFG_REG,0X80);	//INT���ŵ͵�ƽ��Ч
	res=GY_Read_Byte(GY_DEVICE_ID_REG);
	if(res==GY_ADDR)//����ID��ȷ
	{
		GY_Write_Byte(GY_PWR_MGMT1_REG,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		GY_Write_Byte(GY_PWR_MGMT2_REG,0X00);	//���ٶ��������Ƕ�����
		GY_Set_Rate(50);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
}

