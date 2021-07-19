#include "STM32_ADC.h"
#include "STM32_head.h"
/***********************************
file��STM32_ADC.c
���ߣ������
ʱ�䣺2019/7/15
���������������ѧǶ��ʽϵͳʵ����ʹ��

***********************************/


//       ADC      ͨ����    ��Ӧ����
//    ADC1/ADC2    IN0        PA0  
//    ADC1/ADC2    IN1        PA1 
//    ADC1/ADC2    IN2        PA2 
//    ADC1/ADC2    IN3        PA3
//    ADC1/ADC2    IN4        PA4
//    ADC1/ADC2    IN5        PA5
//    ADC1/ADC2    IN6        PA6
//    ADC1/ADC2    IN7        PA7
//    ADC1/ADC2    IN8        PB0
//    ADC1/ADC2    IN9        PB1
//    ADC1/ADC2    IN10       PC0
//    ADC1/ADC2    IN11       PC1
//    ADC1/ADC2    IN12       PC2
//    ADC1/ADC2    IN13       PC3
//    ADC1/ADC2    IN14       PC4
//    ADC1/ADC2    IN15       PC5
//*********************************//

uint8_t CHx[38]=
{
	 0,  1,  2,  3,  4,  5,  6,  7,//A7
	 0,  0,  0,  0,  0,  0,  0,  0,//A15
	 8,  9,  0,  0,  0,  0,  0,  0,//B7
	 0,  0,  0,  0,  0,  0,  0,  0,//B15
	10, 11, 12, 13, 14, 15,        //C5
};


/************************************
������:ADC1_init
������ADC1��ʼ��
���룺Pin(�˿�λ)		(ֻ������ ��12�� ��ʼ�оٵ����ſ���)
�������
������ADC1_init(A8);//
***********************************/
void ADC1_init(PIN_enum pin)
{
	//1.����PinΪģ������//
	gpio_init(pin,0,0);
	
	//2.ʹ��ADC1ʱ�ӣ������÷�Ƶ����//
	RCC->APB2ENR|=(1<<9);//ʹ��ADC1ʱ��
	RCC->APB2RSTR|=1<<9;//��λADC1
	RCC->APB2RSTR&=~(1<<9);//��λ����
	
	RCC->CFGR&=0xffff3fff;
	//RCC->CFGR&=~(2<<14);//2��Ƶʱ��
	//RCC->CFGR|=1<<14;//4��Ƶʱ��
	RCC->CFGR|=2<<14;//6��Ƶʱ�ӣ�72/6=12MHZ��ADC1ʱ��12MHZ
	//RCC->CFGR|=3<<14;//8��Ƶʱ��
	
	//3.����ADC1������ʽ//
	ADC1->CR1&=0XF0FFFF;   //����ģʽ����	
	ADC1->CR1|=0<<16;//����ģʽ�����ÿ��Ź������ü��ģʽ��һͨ��
	ADC1->CR1&=~(1<<8);    //��ɨ��ģʽ	  
	ADC1->CR2=(1<<20)|(7<<17)|(0<<11)|(0<<1);//ʹ���ⲿ�¼�����ת����SWSTART�������ת�����Ҷ��룻����ת��ģʽ
	ADC1->SQR1&=0x000fffff;//һ��ת����ͨ������Ϊ1
//ѡ��239.5���ڣ���������Խ����׼ȷ��Խ��

	if(CHx[pin]<=9)
	{
		ADC1->SMPR2&=~(7<<(3*CHx[pin]));   //ͨ��t����ʱ�����	  
		ADC1->SMPR2|=7<<(3*CHx[pin]);//��������239.5����
	}
	else
	{
		CHx[pin]-=9;
		ADC1->SMPR1&=~(7<<(3*CHx[pin]));   //ͨ��t����ʱ�����	  
		ADC1->SMPR1|=7<<(3*CHx[pin]);//��������239.5����
	}
	ADC1->CR2|=1<<0;//����ADCת����
	ADC1->CR2|=1<<3;//��λУ��
	while(ADC1->CR2&1<<3);//�ȴ�Ч��ɹ�
	ADC1->CR2|=1<<2;//A/D����
	while(ADC1->CR2&1<<2);//�ȴ�Ч��ɹ�
}
/************************************
������:ADC2_init
������ADC2��ʼ��    (ֻ������ ��12�� ��ʼ�оٵ����ſ���)
���룺Pin(�˿�λ)		
�������
������ADC2_init(A8);//��ʼ��ADC2��ͨ��8
***********************************/
void ADC2_init(PIN_enum pin)
{
	//1.����PinΪģ������//
	gpio_init(pin,0,0);
	
	//2.ʹ��ADC2ʱ�ӣ������÷�Ƶ����//
	RCC->APB2ENR|=(1<<10);//ʹ��ADC2ʱ��
	RCC->APB2RSTR|=1<<10;//��λADC2
	RCC->APB2RSTR&=~(1<<10);//��λ����
	
	RCC->CFGR&=0xffff3fff;
	//RCC->CFGR&=~(2<<14);//2��Ƶʱ��
	//RCC->CFGR|=1<<14;//4��Ƶʱ��
	RCC->CFGR|=2<<14;//6��Ƶʱ�ӣ�72/6=12MHZ��ADC1ʱ��12MHZ
	//RCC->CFGR|=3<<14;//8��Ƶʱ��
	
	//3.����ADC2������ʽ//
	ADC2->CR1&=0XF0FFFF;   //����ģʽ����	
	ADC2->CR1|=0<<16;//����ģʽ�����ÿ��Ź������ü��ģʽ��һͨ��
	ADC2->CR1&=~(1<<8);    //��ɨ��ģʽ	  
	ADC2->CR2=(1<<20)|(7<<17)|(0<<11)|(0<<1);//ʹ���ⲿ�¼�����ת����SWSTART�������ת�����Ҷ��룻����ת��ģʽ
	ADC2->SQR1&=0x000fffff;//һ��ת����ͨ������Ϊ1
//ѡ��239.5���ڣ���������Խ����׼ȷ��Խ��

	if(CHx[pin]<=9)
	{
		ADC2->SMPR2&=~(7<<(3*CHx[pin]));   //ͨ��t����ʱ�����	  
		ADC2->SMPR2|=7<<(3*CHx[pin]);//��������239.5����
	}
	else
	{
		CHx[pin]-=9;
		ADC2->SMPR1&=~(7<<(3*CHx[pin]));   //ͨ��t����ʱ�����	  
		ADC2->SMPR1|=7<<(3*CHx[pin]);//��������239.5����
	}
	ADC2->CR2|=1<<0;//����ADCת����
	ADC2->CR2|=1<<3;//��λУ��
	while(ADC2->CR2&1<<3);//�ȴ�Ч��ɹ�
	ADC2->CR2|=1<<2;//A/D����
	while(ADC2->CR2&1<<2);//�ȴ�Ч��ɹ�
}
/************************************
������:get_ADC1
��������ȡADC1ͨ����ֵ  (ֻ������ ��12�� ��ʼ�оٵ����ſ���)
���룺Pin(�˿�λ)		
�����u16
������get_ADC1(A8);//��ȡADC2��ͨ��8��ֵ
***********************************/
u16 get_ADC1(PIN_enum pin)//��ȡADCֵ,t->ͨ����
{
	ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��t
	ADC1->SQR3|=CHx[pin];	
	ADC1->CR2|=1<<22;//����ת������ͨ��
	while(!(ADC1->SR&1<<1));//�ȴ�ת�����
	return ADC1->DR;//����ת�����
}
/************************************
������:get_ADC2
��������ȡADC2ͨ����ֵ  (ֻ������ ��12�� ��ʼ�оٵ����ſ���)
���룺Pin(�˿�λ)		
�����u16
������get_ADC2(A8);//��ȡADC2��ͨ��8��ֵ
***********************************/
u16 get_ADC2(PIN_enum pin)//��ȡADCֵ,t->ͨ����
{
	ADC2->SQR3&=0XFFFFFFE0;//��������1 ͨ��t
	ADC2->SQR3|=CHx[pin];	
	ADC2->CR2|=1<<22;//����ת������ͨ��
	while(!(ADC2->SR&1<<1));//�ȴ�ת�����
	return ADC2->DR;//����ת�����
}
/************************************
������:show_ADC1
��������ӡADC1ͨ����ֵ  (ֻ������ ��12�� ��ʼ�оٵ����ſ���)
���룺Pin(�˿�λ)		
�������
������show_ADC1(A8);//��ӡADC1��ͨ��8��ֵ
***********************************/
void show_ADC1(USART_TypeDef * USARTx,PIN_enum pin)
{
	float t;
	u16 l;
	USART_Sent_String(USARTx,"��ǰ��ѹ��",0);
	t=(float)get_ADC1(pin)*(3.3/4096);//�ɼ�ʮ��adcֵȡ��ֵ
	l=t*1000;	
	USART_Sent_char(USARTx,l/1000+0x30);
	USART_Sent_char(USARTx,'.');
	USART_Sent_char(USARTx,l%1000/100+0x30);
	USART_Sent_char(USARTx,l%100/10+0x30);
	USART_Sent_char(USARTx,l%10+0x30);
	USART_Sent_String(USARTx,"V",1);
	delay_ms(1000);
}
/************************************
������:show_ADC2
��������ӡADC2ͨ����ֵ  (ֻ������ ��12�� ��ʼ�оٵ����ſ���)
���룺Pin(�˿�λ)		
�������
������show_ADC2(A8);//��ӡADC2��ͨ��8��ֵ
***********************************/
void show_ADC2(USART_TypeDef * USARTx,PIN_enum pin)
{
	float t;
	u16 l;
	USART_Sent_String(USARTx,"��ǰ��ѹ��",0);
	t=(float)get_ADC2(pin)*(3.3/4096);//�ɼ�ʮ��adcֵȡ��ֵ
	l=t*1000;	
	USART_Sent_char(USARTx,l/1000+0x30);
	USART_Sent_char(USARTx,'.');
	USART_Sent_char(USARTx,l%1000/100+0x30);
	USART_Sent_char(USARTx,l%100/10+0x30);
	USART_Sent_char(USARTx,l%10+0x30);
	USART_Sent_String(USARTx,"V",1);
	delay_ms(1000);
}




