#include "STM32_ADC.h"
#include "STM32_head.h"
/***********************************
file：STM32_ADC.c
作者：廖凌峰
时间：2019/7/15
本代码仅供北华大学嵌入式系统实验室使用

***********************************/


//       ADC      通道号    对应引脚
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
函数名:ADC1_init
描述：ADC1初始化
输入：Pin(端口位)		(只有上述 第12行 开始列举的引脚可用)
输出：无
样例：ADC1_init(A8);//
***********************************/
void ADC1_init(PIN_enum pin)
{
	//1.设置Pin为模拟输入//
	gpio_init(pin,0,0);
	
	//2.使能ADC1时钟，并设置分频因子//
	RCC->APB2ENR|=(1<<9);//使能ADC1时钟
	RCC->APB2RSTR|=1<<9;//复位ADC1
	RCC->APB2RSTR&=~(1<<9);//复位结束
	
	RCC->CFGR&=0xffff3fff;
	//RCC->CFGR&=~(2<<14);//2分频时钟
	//RCC->CFGR|=1<<14;//4分频时钟
	RCC->CFGR|=2<<14;//6分频时钟，72/6=12MHZ，ADC1时钟12MHZ
	//RCC->CFGR|=3<<14;//8分频时钟
	
	//3.设置ADC1工作方式//
	ADC1->CR1&=0XF0FFFF;   //工作模式清零	
	ADC1->CR1|=0<<16;//独立模式，禁用看门狗，禁用间断模式，一通道
	ADC1->CR1&=~(1<<8);    //非扫描模式	  
	ADC1->CR2=(1<<20)|(7<<17)|(0<<11)|(0<<1);//使用外部事件启动转换；SWSTART软件控制转换；右对齐；单次转换模式
	ADC1->SQR1&=0x000fffff;//一个转换，通道个数为1
//选用239.5周期，所用周期越长，准确性越高

	if(CHx[pin]<=9)
	{
		ADC1->SMPR2&=~(7<<(3*CHx[pin]));   //通道t采样时间清空	  
		ADC1->SMPR2|=7<<(3*CHx[pin]);//采样周期239.5周期
	}
	else
	{
		CHx[pin]-=9;
		ADC1->SMPR1&=~(7<<(3*CHx[pin]));   //通道t采样时间清空	  
		ADC1->SMPR1|=7<<(3*CHx[pin]);//采样周期239.5周期
	}
	ADC1->CR2|=1<<0;//开启ADC转换器
	ADC1->CR2|=1<<3;//复位校验
	while(ADC1->CR2&1<<3);//等待效验成功
	ADC1->CR2|=1<<2;//A/D检验
	while(ADC1->CR2&1<<2);//等待效验成功
}
/************************************
函数名:ADC2_init
描述：ADC2初始化    (只有上述 第12行 开始列举的引脚可用)
输入：Pin(端口位)		
输出：无
样例：ADC2_init(A8);//初始化ADC2的通道8
***********************************/
void ADC2_init(PIN_enum pin)
{
	//1.设置Pin为模拟输入//
	gpio_init(pin,0,0);
	
	//2.使能ADC2时钟，并设置分频因子//
	RCC->APB2ENR|=(1<<10);//使能ADC2时钟
	RCC->APB2RSTR|=1<<10;//复位ADC2
	RCC->APB2RSTR&=~(1<<10);//复位结束
	
	RCC->CFGR&=0xffff3fff;
	//RCC->CFGR&=~(2<<14);//2分频时钟
	//RCC->CFGR|=1<<14;//4分频时钟
	RCC->CFGR|=2<<14;//6分频时钟，72/6=12MHZ，ADC1时钟12MHZ
	//RCC->CFGR|=3<<14;//8分频时钟
	
	//3.设置ADC2工作方式//
	ADC2->CR1&=0XF0FFFF;   //工作模式清零	
	ADC2->CR1|=0<<16;//独立模式，禁用看门狗，禁用间断模式，一通道
	ADC2->CR1&=~(1<<8);    //非扫描模式	  
	ADC2->CR2=(1<<20)|(7<<17)|(0<<11)|(0<<1);//使用外部事件启动转换；SWSTART软件控制转换；右对齐；单次转换模式
	ADC2->SQR1&=0x000fffff;//一个转换，通道个数为1
//选用239.5周期，所用周期越长，准确性越高

	if(CHx[pin]<=9)
	{
		ADC2->SMPR2&=~(7<<(3*CHx[pin]));   //通道t采样时间清空	  
		ADC2->SMPR2|=7<<(3*CHx[pin]);//采样周期239.5周期
	}
	else
	{
		CHx[pin]-=9;
		ADC2->SMPR1&=~(7<<(3*CHx[pin]));   //通道t采样时间清空	  
		ADC2->SMPR1|=7<<(3*CHx[pin]);//采样周期239.5周期
	}
	ADC2->CR2|=1<<0;//开启ADC转换器
	ADC2->CR2|=1<<3;//复位校验
	while(ADC2->CR2&1<<3);//等待效验成功
	ADC2->CR2|=1<<2;//A/D检验
	while(ADC2->CR2&1<<2);//等待效验成功
}
/************************************
函数名:get_ADC1
描述：获取ADC1通道的值  (只有上述 第12行 开始列举的引脚可用)
输入：Pin(端口位)		
输出：u16
样例：get_ADC1(A8);//获取ADC2的通道8的值
***********************************/
u16 get_ADC1(PIN_enum pin)//获取ADC值,t->通道号
{
	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道t
	ADC1->SQR3|=CHx[pin];	
	ADC1->CR2|=1<<22;//开启转换规则通道
	while(!(ADC1->SR&1<<1));//等待转换完成
	return ADC1->DR;//返回转换结果
}
/************************************
函数名:get_ADC2
描述：获取ADC2通道的值  (只有上述 第12行 开始列举的引脚可用)
输入：Pin(端口位)		
输出：u16
样例：get_ADC2(A8);//获取ADC2的通道8的值
***********************************/
u16 get_ADC2(PIN_enum pin)//获取ADC值,t->通道号
{
	ADC2->SQR3&=0XFFFFFFE0;//规则序列1 通道t
	ADC2->SQR3|=CHx[pin];	
	ADC2->CR2|=1<<22;//开启转换规则通道
	while(!(ADC2->SR&1<<1));//等待转换完成
	return ADC2->DR;//返回转换结果
}
/************************************
函数名:show_ADC1
描述：打印ADC1通道的值  (只有上述 第12行 开始列举的引脚可用)
输入：Pin(端口位)		
输出：无
样例：show_ADC1(A8);//打印ADC1的通道8的值
***********************************/
void show_ADC1(USART_TypeDef * USARTx,PIN_enum pin)
{
	float t;
	u16 l;
	USART_Sent_String(USARTx,"当前电压：",0);
	t=(float)get_ADC1(pin)*(3.3/4096);//采集十组adc值取均值
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
函数名:show_ADC2
描述：打印ADC2通道的值  (只有上述 第12行 开始列举的引脚可用)
输入：Pin(端口位)		
输出：无
样例：show_ADC2(A8);//打印ADC2的通道8的值
***********************************/
void show_ADC2(USART_TypeDef * USARTx,PIN_enum pin)
{
	float t;
	u16 l;
	USART_Sent_String(USARTx,"当前电压：",0);
	t=(float)get_ADC2(pin)*(3.3/4096);//采集十组adc值取均值
	l=t*1000;	
	USART_Sent_char(USARTx,l/1000+0x30);
	USART_Sent_char(USARTx,'.');
	USART_Sent_char(USARTx,l%1000/100+0x30);
	USART_Sent_char(USARTx,l%100/10+0x30);
	USART_Sent_char(USARTx,l%10+0x30);
	USART_Sent_String(USARTx,"V",1);
	delay_ms(1000);
}




