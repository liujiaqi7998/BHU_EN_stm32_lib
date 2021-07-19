/************************************
file��myiic.c
���ߣ��½�
ʱ�䣺2019/7/17
���������������ѧǶ��ʽϵͳʵ����ʹ��
************************************/
#include "myiic.h"
#include "STM32_head.h"



/************************************
������: IIC_Init
����: ��ʼ��IIC
����: ��
���: ��
����: IIC_Init();
************************************/
void IIC_Init(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{					     
 	//RCC->APB2ENR|=1<<3;		//��ʹ������IO PORTBʱ�� 							 
	//GPIOB->CRL&=0X00FFFFFF;	
	//GPIOB->CRL|=0X33000000;	  //PB6/7 �������
	//GPIOB->ODR|=3<<6;     	//PB6,7 �����
	
	gpio_init(IIC_SCL,3,0);
	gpio_init(IIC_SDA,3,0);
}
/************************************
������: IIC_Start
����: ����IIC��ʼ�ź�
����: ��
���: ��
����: IIC_Start();
************************************/
void IIC_Start(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	//SDA_OUT();     //sda�����
	gpio_Set(IIC_SDA,3,0);
	//IIC_SDA=1;
	gpio_Setstate(IIC_SDA,1);	  	  
	//IIC_SCL=1;
	gpio_Setstate(IIC_SCL,1);
	delay_us(4);
 	//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	gpio_Setstate(IIC_SDA,0);	  
	delay_us(4);
	//IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
	gpio_Setstate(IIC_SCL,0); 
}	
/************************************
������: IIC_Stop
����: ����IICֹͣ�ź�
����: ��
���: ��
����: IIC_Stop();
************************************/
void IIC_Stop(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	//SDA_OUT();//sda�����
	gpio_Set(IIC_SDA,3,0);
	//IIC_SCL=0;
	gpio_Setstate(IIC_SCL,0); 
	//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	gpio_Setstate(IIC_SDA,0);
 	delay_us(4);
	//IIC_SCL=1; 
	gpio_Setstate(IIC_SCL,1);
	//IIC_SDA=1;//����I2C���߽����ź�
	gpio_Setstate(IIC_SDA,1);
	delay_us(4);							   	
}
/************************************
������: IIC_Wait_Ack
����: �ȴ�Ӧ���źŵ���
����: ��
���: 1������Ӧ��ʧ�� 0������Ӧ��ɹ�
����: IIC_Wait_Ack();
************************************/
u8 IIC_Wait_Ack(PIN_enum IIC_SCL,PIN_enum IIC_SDA)
{
	u8 ucErrTime=0;
	//SDA_IN();      //SDA����Ϊ����  
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
	//IIC_SCL=0;//ʱ�����0
	gpio_Setstate(IIC_SCL,0);	   
	return 0;  
} 
/************************************
������: IIC_Ack
����: ����ACKӦ��
����: ��
���: ��
����: IIC_Ack();
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
������: IIC_NAck
����: ������ACKӦ��
����: ��
���: ��
����: IIC_NAck();
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
������: IIC_Send_Byte
����: IIC����һ���ֽ�
����: txd:һ���ֽ�
���: 1����Ӧ�� 0����Ӧ��
����: IIC_Send_Byte(txd);
************************************/
void IIC_Send_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,u8 txd)
{                        
    u8 t;   
	//SDA_OUT(); 	
	gpio_Set(IIC_SDA,3,0);    
    //IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    gpio_Setstate(IIC_SCL,0);
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
        gpio_Setstate(IIC_SDA,(txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		//IIC_SCL=1;
		gpio_Setstate(IIC_SCL,1);
		delay_us(2); 
		//IIC_SCL=0;
		gpio_Setstate(IIC_SCL,0);	
		delay_us(2);
    }	 
} 	  
/************************************
������: IIC_Read_Byte
����: IIC��ȡ1���ֽ�
����: ack: ack=1ʱ������ACK��ack=0������nACK   
���: ����ֵ
����: IIC_Read_Byte(ack);
************************************/
u8 IIC_Read_Byte(PIN_enum IIC_SCL,PIN_enum IIC_SDA,unsigned char ack)
{
	unsigned char i,receive=0;
	//SDA_IN();//SDA����Ϊ����
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
        IIC_NAck(IIC_SCL,IIC_SDA);//����nACK
    else
        IIC_Ack(IIC_SCL,IIC_SDA); //����ACK   
    return receive;
}



























