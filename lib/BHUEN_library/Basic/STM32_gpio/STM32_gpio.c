#include "STM32_gpio.h"
/***********************************
file：STM32_gpio.c
作者：廖凌峰
时间：2019/7/15
本代码仅供北华大学嵌入式系统实验室使用
***********************************/

//GPIOx可填GPIOA~GPIOE//
//GPIO_Pin可填0~15//
/************************************
函数名:gpio_init
描述：GPIO端口初始化(默认输出为高)
输入：Pin(端口位),
GPIO_Speed(速度;
					输入模式：0:(复位状态)
					输出模式：1:最大10MHZ 2:最大2MHZ 3:最大50MHZ)
			GPIO_Mode(
					输入模式：0:模拟输入1:浮空输入(复位状态)2:上下拉输入3:保留
					输出模式: 0:推挽输出1:开漏输出2:复用推挽3:复用开漏)
			
输出：无
样例：gpio_init(A8,3,0);//A8,50MHZ,推挽输出
***********************************/
void gpio_init(PIN_enum pin,uint16_t GPIO_Speed,uint16_t GPIO_Mode)
{
	GPIO_init(change_pin(pin),pin%16,GPIO_Mode,GPIO_Speed);
}
/************************************
函数名:GPIO_init_All_like
描述：GPIO端口局部初始化
输入：GPIOx(端口)
			GPIO_Mode(输入模式：0:模拟输入1:浮空输入(复位状态)2:上下拉输入3:保留
								输出模式: 0:推挽输出1:开漏输出2:复用推挽3:复用开漏)
			GPIO_Speed(速度;输入模式：0:(复位状态)输出模式：1:最大10MHZ 2:最大2MHZ 3:最大50MHZ)
			temp(二进制一位数表示一个IO口位置)
输出：无
样例：GPIO_init_All_like(GPIOA,0,3,0xfe);//GPIOA,推挽输出，50MHZ,
***********************************/
void GPIO_init_All_like(GPIO_TypeDef* GPIOx,uint16_t GPIO_Mode,uint16_t GPIO_Speed,uint16_t temp)
{
	int i=0;
	for(i=0;i<16;i++)
	{
		if(temp&0x01)
			GPIO_init(GPIOx,i,GPIO_Mode,GPIO_Speed);
		temp>>=1;
	}
}
/************************************
函数名:gpio_GetIO
描述：GPIO端口状态获取
输入：Pin(端口位)
输出：1或0
样例：
uint8_t i=0;
i=gpio_GetIO(A8);//获取GPIOA,端口8的值
***********************************/
uint8_t gpio_GetIO(PIN_enum pin)
{
	return GPIO_GetIO(change_pin(pin),pin%16);
}
/************************************
函数名:gpio_Set
描述：GPIO端口状态修改
输入：Pin(端口位),
GPIO_Speed(速度;
					输入模式：0:(复位状态)
					输出模式：1:最大10MHZ 2:最大2MHZ 3:最大50MHZ)
			GPIO_Mode(
					输入模式：0:模拟输入1:浮空输入(复位状态)2:上下拉输入3:保留
					输出模式: 0:推挽输出1:开漏输出2:复用推挽3:复用开漏)
			
输出：无
样例gpio_Set(A8,3,0);//A8,50MHZ,推挽输出
************************************/
void gpio_Set(PIN_enum pin,uint16_t GPIO_Speed,uint16_t GPIO_Mode)
{
	GPIO_Set(change_pin(pin),pin%16,GPIO_Mode,GPIO_Speed);
}
/************************************
函数名:gpio_Setstate
描述：GPIO端口状态设置
输入：Pin(端口位)，state(端口状态)
输出：无
样例：gpio_Setstate(A8,0);//A8,输出0
***********************************/
void gpio_Setstate(PIN_enum pin, uint8_t state)
{
	GPIO_Setstate(change_pin(pin),pin%16,state);
}
/************************************
函数名:gpio_SetIO
描述：GPIO端口状态反转
输入：Pin(端口位)
输出：无
样例：gpio_SetIO(A8);//GPIOA,端口8,复位
***********************************/
void gpio_SetIO(PIN_enum pin)
{
	GPIO_SetIO(change_pin(pin),pin%16);
}

PIN_enum show_pin(char i,char j)
{
	PIN_enum * m;
	char t=i*16+j-1;
	while(t--)
	{
		m++;
	}
	return *m;
}





/************************************
(系统自动调用，用户不需要查看)
***********************************/
GPIO_TypeDef* change_pin(PIN_enum pin)
{
	if(pin/16==0)//GPIOA
		return GPIOA;
	else if(pin/16==1)//GPIOB
		return GPIOB;
	else if(pin/16==2)//GPIOC
		return GPIOC;
	else if(pin/16==3)//GPIOD
		return GPIOD;
	else if(pin/16==4)//GPIOE
		return GPIOE;
	return 0;
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_init
描述：GPIO端口初始化(默认输出为高，修改请找本函数最后一句)
输入：GPIOx(端口)，GPIO_Pin(端口位),
			GPIO_Mode(输入模式：0:模拟输入1:浮空输入(复位状态)2:上下拉输入3:保留
								输出模式: 0:推挽输出1:开漏输出2:复用推挽3:复用开漏)
			GPIO_Speed(速度;输入模式：0:(复位状态)输出模式：1:最大10MHZ 2:最大2MHZ 3:最大50MHZ)
输出：无
样例：GPIO_init(GPIOA,8,0,3);//GPIOA,端口8,推挽输出，50MHZ
***********************************/
void GPIO_init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,uint16_t GPIO_Mode,uint16_t GPIO_Speed)
{
	uint32_t j=0;
	uint32_t i=0;
	i=((uint32_t)GPIOx-APB2PERIPH_BASE)/0x0400;
	RCC->APB2ENR|=(1<<i);
	if(GPIO_Pin>=8)//端口8-15
	{
		j=(GPIO_Mode<<(((GPIO_Pin-8)<<2)+2))|(GPIO_Speed<<((GPIO_Pin-8)<<2));
		GPIOx->CRH&=~(0x000f<<((GPIO_Pin-8)<<2));
		GPIOx->CRH|=j;
	}
	else//端口0-7
	{
		j=(GPIO_Mode<<((GPIO_Pin<<2)+2))|(GPIO_Speed<<(GPIO_Pin<<2));
		GPIOx->CRL&=~(0x000f<<(GPIO_Pin<<2));
		GPIOx->CRL|=j;
	}
	GPIOx->ODR&=~(1<<GPIO_Pin);
	GPIOx->ODR|=(1<<GPIO_Pin);//屏蔽此句即可默认为低
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_init_PinAll
描述：GPIO端口整体初始化
输入：GPIOx(端口)
			GPIO_Mode(输入模式：0:模拟输入1:浮空输入(复位状态)2:上下拉输入3:保留
								输出模式: 0:推挽输出1:开漏输出2:复用推挽3:复用开漏)
			GPIO_Speed(速度;输入模式：0:(复位状态)输出模式：1:最大10MHZ 2:最大2MHZ 3:最大50MHZ)
输出：无
样例：GPIO_init_PinAll(GPIOA,0,3);//GPIOA,推挽输出，50MHZ
***********************************/
void GPIO_init_PinAll(GPIO_TypeDef* GPIOx,uint16_t GPIO_Mode,uint16_t GPIO_Speed)
{
	int i=0;
	for(i=0;i<16;i++)
	{
		GPIO_init(GPIOx,i,GPIO_Mode,GPIO_Speed);
	}
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_Set
描述：GPIO端口状态修改
输入：GPIOx(端口)，GPIO_Pin(端口位),
			GPIO_Mode(输入模式：0:模拟输入1:浮空输入(复位状态)2:上下拉输入3:保留
								输出模式: 0:推挽输出1:开漏输出2:复用推挽3:复用开漏)
			GPIO_Speed(速度;输入模式：0:(复位状态)输出模式：1:最大10MHZ 2:最大2MHZ 3:最大50MHZ)
输出：无
样例：GPIO_Set(GPIOA,8,0,3);//GPIOA,端口8,推挽输出，50MHZ
************************************/
void GPIO_Set(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,uint16_t GPIO_Mode,uint16_t GPIO_Speed)
{
	uint32_t j=0;
	uint32_t i=0;
	i=((uint32_t)GPIOx-APB2PERIPH_BASE)/0x0400;
	RCC->APB2ENR|=(1<<i);
	if(GPIO_Pin>=8)//端口8-15
	{
		j=(GPIO_Mode<<(((GPIO_Pin-8)<<2)+2))|(GPIO_Speed<<((GPIO_Pin-8)<<2));
		GPIOx->CRH&=~(0x000f<<((GPIO_Pin-8)<<2));
		GPIOx->CRH|=j;
	}
	else//端口0-7
	{
		j=(GPIO_Mode<<((GPIO_Pin<<2)+2))|(GPIO_Speed<<(GPIO_Pin<<2));
		GPIOx->CRL&=~(0x000f<<(GPIO_Pin<<2));
		GPIOx->CRL|=j;
	}
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_Set_Speed
描述：GPIO端口速度修改
输入：GPIOx(端口)，GPIO_Pin(端口位)
			GPIO_Speed(速度;输入模式：0:(复位状态)输出模式：1:最大10MHZ 2:最大2MHZ 3:最大50MHZ)
输出：无
样例：GPIO_Set_Speed(GPIOA,8,2);//GPIOA,端口8,2MHZ
***********************************/
void GPIO_Set_Speed(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t GPIO_Speed)
{
	uint32_t j=0;
	if(GPIO_Pin>=8)//端口8-15
	{
		j=GPIO_Speed<<((GPIO_Pin-8)<<2);
		GPIOx->CRH&=~(0x0003<<((GPIO_Pin-8)<<2));
		GPIOx->CRH|=j;
	}
	else//端口0-7
	{
		j=GPIO_Speed<<(GPIO_Pin<<2);
		GPIOx->CRL&=~(0x0003<<(GPIO_Pin<<2));
		GPIOx->CRL|=j;
	}
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_Setstate
描述：GPIO端口状态设置
输入：GPIOx(端口)，GPIO_Pin(端口位)，state(端口状态)
输出：无
样例：GPIO_Setstate(GPIOA,8,0);//GPIOA,端口8,输出0
***********************************/
void GPIO_Setstate(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin, uint8_t state)
{
	if(state)
	{
		GPIOx->ODR|=(1<<GPIO_Pin);
	}
	else
	{		
		GPIOx->ODR&=~(1<<GPIO_Pin);
	}
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_GetIO
描述：GPIO端口状态获取
输入：GPIOx(端口)，GPIO_Pin(端口位)
输出：1或0
样例：
uint8_t i=0;
i=GPIO_GetIO(GPIOA,8);//获取GPIOA,端口8的值
***********************************/
uint8_t GPIO_GetIO(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	if(GPIOx->IDR&(1<<GPIO_Pin))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_SetIO
描述：GPIO端口状态反转
输入：GPIOx(端口)，GPIO_Pin(端口位)
输出：无
样例：GPIO_SetIO(GPIOA,8);//GPIOA,端口8,复位
***********************************/
void GPIO_SetIO(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	if(GPIOx->ODR&(1<<GPIO_Pin))
	{
		GPIOx->ODR&=~(1<<GPIO_Pin);
	}
	else
	{
		GPIOx->ODR|=(1<<GPIO_Pin);
	}
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_USART_IOint
描述：GPIO串口IO初始化
输入：
输出：无
样例：GPIO_USART_IOint(USART1);//初始化串口1  IO
***********************************/
void GPIO_USART_IOint(USART_TypeDef * USARTx)
{
	if(USARTx==USART1)
	{
		RCC->APB2ENR|=(1<<14);//使能USART1时钟
		GPIO_Set(GPIOA,9,2,3);
		GPIO_Set(GPIOA,10,2,0);
		RCC->APB2RSTR|=(1<<14);//复位USART1
		RCC->APB2RSTR&=~(1<<14);//关闭复位，复位完成
	}
	else if(USARTx==USART2)
	{
		RCC->APB1ENR|=(1<<17);//使能USART2时钟
		GPIO_Set(GPIOA,2,2,3);
		GPIO_Set(GPIOA,3,2,0);
		RCC->APB1RSTR|=(1<<17);//复位USART1
		RCC->APB1RSTR&=~(1<<17);//关闭复位，复位完成
	}
	else if(USARTx==USART3)
	{
		RCC->APB1ENR|=(1<<18);//使能USART3时钟
		GPIO_Set(GPIOB,10,2,3);
		GPIO_Set(GPIOB,11,2,0);
		RCC->APB1RSTR|=(1<<18);//复位USART1
		RCC->APB1RSTR&=~(1<<18);//关闭复位，复位完成
	}
}
/************************************
(系统自动调用，用户不需要查看)
函数名:GPIO_TIM_IOint
描述：GPIO定时器IO初始化
输入：
输出：无
样例：GPIO_TIM_IOint(USART1);//初始化串口1  IO
***********************************/
void GPIO_TIM_IOint(TIM_TypeDef * TIMx,uint8_t CHx)
{
	if(TIMx==TIM1)
	{
		RCC->APB2ENR|=1<<11;//使能定时器1时钟
		if(CHx==1)
			GPIO_Set(GPIOA,8,2,3);
		else if(CHx==2)
			GPIO_Set(GPIOA,9,2,3);
		else if(CHx==3)
			GPIO_Set(GPIOA,10,2,3);
		else if(CHx==4)
			GPIO_Set(GPIOA,11,2,3);
		RCC->APB2ENR|=1<<0;//开启辅助时钟
		AFIO->MAPR&=~(3<<6);//无重映射
	}
	else if(TIMx==TIM2)
	{
		RCC->APB1ENR|=1<<0;//使能定时器2时钟
		if(CHx==1)
			GPIO_Set(GPIOA,0,2,3);
		else if(CHx==2)
			GPIO_Set(GPIOA,1,2,3);
		else if(CHx==3)
			GPIO_Set(GPIOA,2,2,3);
		else if(CHx==4)
			GPIO_Set(GPIOA,3,2,3);
		RCC->APB2ENR|=1<<0;//开启辅助时钟
		AFIO->MAPR&=~(3<<8);//无重映射
	}
	else if(TIMx==TIM3)
	{
		RCC->APB1ENR|=1<<1;//使能定时器3时钟
		if(CHx==1)
			GPIO_Set(GPIOA,6,2,3);
		else if(CHx==2)
			GPIO_Set(GPIOA,7,2,3);
		else if(CHx==3)
			GPIO_Set(GPIOB,0,2,3);
		else if(CHx==4)
			GPIO_Set(GPIOB,1,2,3);
		RCC->APB2ENR|=1<<0;//开启辅助时钟
		AFIO->MAPR&=~(3<<10);//无重映射
	}
	else if(TIMx==TIM4)
	{
		RCC->APB1ENR|=1<<2;//使能定时器4时钟
		if(CHx==1)
			GPIO_Set(GPIOB,6,2,3);
		else if(CHx==2)
			GPIO_Set(GPIOB,7,2,3);
		else if(CHx==3)
			GPIO_Set(GPIOB,8,2,3);
		else if(CHx==4)
			GPIO_Set(GPIOB,9,2,3);
		RCC->APB2ENR|=1<<0;//开启辅助时钟
		AFIO->MAPR&=~(1<<12);//无重映射
	}
	else if(TIMx==TIM5)
	{
		RCC->APB1ENR|=1<<3;//使能定时器5时钟
		if(CHx==1)
			GPIO_Set(GPIOA,0,2,3);
		else if(CHx==2)
			GPIO_Set(GPIOA,1,2,3);
		else if(CHx==3)
			GPIO_Set(GPIOA,2,2,3);
		else if(CHx==4)
			GPIO_Set(GPIOA,3,2,3);
		RCC->APB2ENR|=1<<0;//开启辅助时钟
		AFIO->MAPR&=~(1<<16);//无重映射
	}
}


