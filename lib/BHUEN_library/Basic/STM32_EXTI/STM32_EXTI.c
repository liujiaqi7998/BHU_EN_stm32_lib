#include "STM32_EXTI.h"
#include "STM32_head.h"
/***********************************
file：STM32_EXTI.c
作者：廖凌峰
时间：2019/7/16
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
//外部中断初始化程序
/************************************
函数名:EXTIX_Init(请自行修改)                 //->   中断处理函数位于  "irq.c"    <-//
描述：外部中断初始化
输入：无
输出：无
样例：EXTIX_Init();//
***********************************/
void EXTIX_Init(void)
{
	//Ex_NVIC_Config(GPIO_A,0,RTIR); 	//上升沿触发(样例1，PA中断0)
	//Ex_NVIC_Config(GPIO_A,0,FTIR); 	//下降沿触发(样例2，PA中断0)
	//Ex_NVIC_Config(GPIO_A,0,RTIR+FTIR); 	//任意电平触发(样例3，PA中断0)
	//Ex_NVIC_Config(GPIO_B,0,RTIR); 	//上升沿触发(PB中断0)
	//Ex_NVIC_Config(GPIO_C,0,RTIR); 	//上升沿触发(PC中断0)
	//Ex_NVIC_Config(GPIO_D,0,RTIR); 	//上升沿触发(PD中断0)
	//Ex_NVIC_Config(GPIO_E,0,RTIR); 	//上升沿触发(PE中断0)
	
	//Ex_NVIC_Config(GPIO_A,0,FTIR); 	//下降沿触发(PE中断1)
	
	//Ex_NVIC_Config(GPIO_E,2,FTIR); 	//下降沿触发(PE中断2)
	
	//Ex_NVIC_Config(GPIO_E,3,FTIR); 	//下降沿触发(PE中断3)
	
 	//Ex_NVIC_Config(GPIO_E,4,FTIR); 	//下降沿触发(PE中断4)
	
	//MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//抢占2，子优先级3，组2(数值越小优先级越高)
	//MY_NVIC_Init(3,3,EXTI1_IRQn,2);	//抢占3，子优先级3，组2
	//MY_NVIC_Init(2,2,EXTI2_IRQn,2);	//抢占2，子优先级2，组2   
	//MY_NVIC_Init(2,1,EXTI3_IRQn,2);	//抢占2，子优先级1，组2	   
	//MY_NVIC_Init(2,0,EXTI4_IRQn,2);	//抢占2，子优先级0，组2	   
}



