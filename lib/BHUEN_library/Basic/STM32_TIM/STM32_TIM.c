#include "STM32_head.h"
#include "STM32_TIM.h"
/***********************************
file：STM32_TIM.c
作者：廖凌峰
时间：2019/7/16
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
//定时器1   72MHZ
//           TIM1_CH1----PA8       //
//           TIM1_CH2----PA9       //
//           TIM1_CH3----PA10      //
//           TIM1_CH4----PA11      //
//定时器2   36MHZ
//           TIM2_CH1----PA0       //
//           TIM2_CH2----PA1       //
//           TIM2_CH3----PA2       //
//           TIM2_CH4----PA3       //
//定时器3   36MHZ
//           TIM3_CH1----PA6       //
//           TIM3_CH2----PA7       //
//           TIM3_CH3----PB0       //
//           TIM3_CH4----PB1       //
//定时器4   36MHZ
//           TIM4_CH1----PB6       //
//           TIM4_CH2----PB7       //
//           TIM4_CH3----PB8       //
//           TIM4_CH4----PB9       //
//定时器5   36MHZ
//           TIM5_CH1----PA0       //
//           TIM5_CH2----PA1       //
//           TIM5_CH3----PA2       //
//           TIM5_CH4----PA3       //

/************************************
函数名:TIM_PWM_init(只能初始化TIM1，TIM2，TIM3，TIM4，TIM5)
描述:定时器pwm初始化（不使用重映射）
输入：TIMx(定时器号),CHx(通道号),psc(预分频系数),arr(周期时间)
输出：无
样例：TIM_PWM_init(TIM3,3,72,20000);//TIM3通道3,10KHZ,20ms,
***********************************/
void TIM_PWM_init(TIM_TypeDef * TIMx,uint8_t CHx,uint32_t psc,uint32_t arr)
{
	GPIO_TIM_IOint(TIMx,CHx);
	TIMx->PSC=psc-1;//  psc=72   72000000/((PSC+1)*ARR)=50HZ 0.001ms 1us
	TIMx->ARR=arr-1;//
	if(CHx==1)
	{
		TIMx->CCR1=(arr/2)-1;//通道1跳变点
		TIMx->CCER|=(1<<0);//输出到对应引脚
		TIMx->CCMR1|=(6<<4);//pwm模式1向上计数,先高
		TIMx->CCMR1|=(1<<3);//预装载使能
	}
	else if(CHx==2)
	{
		TIMx->CCR2=(arr/2)-1;//通道2跳变点
		TIMx->CCER|=(1<<4);//输出到对应引脚
		TIMx->CCMR1|=(6<<12);//pwm模式1向上计数,先高
		TIMx->CCMR1|=(1<<11);//预装载使能
	}
	else if(CHx==3)
	{
		TIMx->CCR3=(arr/2)-1;//通道3跳变点
		TIMx->CCER|=(1<<8);//输出到对应引脚
		TIMx->CCMR2|=(6<<4);//pwm模式1向上计数,先高
		TIMx->CCMR2|=(1<<3);//预装载使能
	}
	else if(CHx==4)
	{
		TIMx->CCR4=(arr/2)-1;//通道4跳变点
		TIMx->CCER|=(1<<12);//输出到对应引脚
		TIMx->CCMR2|=(6<<12);//pwm模式1向上计数,先高
		TIMx->CCMR2|=(1<<11);//预装载使能
	}
	TIMx->CR1=0x0080; //ARPE使能（主动重装载允许位）
	TIMx->CR1|=0x01;//使能定时器
	if(TIMx==TIM1||TIMx==TIM8)//高级计时器需要
	{
		TIMx->BDTR|=(1<<14);
	}
}
/************************************
函数名:PWM_change_CCR1
描述:PWM改变跳变点
输入：TIMx(定时器号),ccr1(跳变点)
输出：无
样例：PWM_change_CCRx(TIM3,3,1500);//TIM3计数1500次跳变
***********************************/
void PWM_change_CCRx(TIM_TypeDef * TIMx,uint8_t CHx,uint16_t ccr1)
{
	if(CHx==1)
		TIMx->CCR1=ccr1-1;//跳变点
	else if(CHx==2)
		TIMx->CCR2=ccr1-1;//跳变点
	else if(CHx==3)
		TIMx->CCR3=ccr1-1;//跳变点
	else if(CHx==4)
		TIMx->CCR4=ccr1-1;//跳变点
}
/************************************
函数名:TIMx_irq_init(中断优先级要在本函数最后一句修改)    
描述:定时器中断初始化                                   //->   中断处理函数位于  "irq.c"    <-//
输入：TIMx(定时器号),arr(自动装载值),psc(分频系数)
输出：无
样例：TIMx_irq_init(TIM3,10000,7200);//TIM3 定时器触发时长 1S    时钟频率 10khz 
***********************************/
void TIMx_irq_init(TIM_TypeDef * TIMx,u16 arr,u16 psc)
{
	if(TIMx==TIM1)
		RCC->APB2ENR|=1<<11;//使能定时器1时钟
	else if(TIMx==TIM2)
		RCC->APB1ENR|=1<<0;//使能定时器2时钟
	else if(TIMx==TIM3)
		RCC->APB1ENR|=1<<1;//使能定时器3时钟
	else if(TIMx==TIM4)
		RCC->APB1ENR|=1<<2;//使能定时器4时钟
	else if(TIMx==TIM5)
		RCC->APB1ENR|=1<<3;//使能定时器5时钟
	
	TIMx->ARR=arr-1;//设置自动装载值    时间=时钟频率*arr
	TIMx->PSC=psc-1;//时钟频率=1/(72000000/psc)
	TIMx->DIER|=1<<0;//允许中断更新
	TIMx->CR1|=0x01;//使能计时器（内部rc）
	
	// if(TIMx==TIM2)
	// 	MY_NVIC_Init(2,2,TIM2_IRQn,2);//(优先级自行修改)
	// else if(TIMx==TIM3)
	// 	MY_NVIC_Init(2,2,TIM3_IRQn,2);
	// else if(TIMx==TIM4)
	// 	MY_NVIC_Init(2,2,TIM4_IRQn,2);
	//else if(TIMx==TIM5)   //STM32f103C8T6没有定时器5,需要屏蔽
	//	MY_NVIC_Init(2,2,TIM5_IRQn,2);
}
/************************************
函数名:TIMx_irq_close  
描述:定时器中断关闭                                  //->   中断处理函数位于  "irq.c"    <-//
输入：TIMx(定时器号)
输出：无
样例：TIMx_irq_close(TIM3);//TIM3 定时器关闭
***********************************/
void TIMx_irq_close(TIM_TypeDef * TIMx)
{
	TIMx->DIER&=~(1<<0);//不允许中断更新
	TIMx->CR1&=~(1<<0);//禁止使能计时器（内部rc）
}

