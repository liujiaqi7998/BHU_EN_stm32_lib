#include "STM32_EXTI.h"
#include "STM32_head.h"
/***********************************
file��STM32_EXTI.c
���ߣ������
ʱ�䣺2019/7/16
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
//�ⲿ�жϳ�ʼ������
/************************************
������:EXTIX_Init(�������޸�)                 //->   �жϴ�����λ��  "irq.c"    <-//
�������ⲿ�жϳ�ʼ��
���룺��
�������
������EXTIX_Init();//
***********************************/
void EXTIX_Init(void)
{
	//Ex_NVIC_Config(GPIO_A,0,RTIR); 	//�����ش���(����1��PA�ж�0)
	//Ex_NVIC_Config(GPIO_A,0,FTIR); 	//�½��ش���(����2��PA�ж�0)
	//Ex_NVIC_Config(GPIO_A,0,RTIR+FTIR); 	//�����ƽ����(����3��PA�ж�0)
	//Ex_NVIC_Config(GPIO_B,0,RTIR); 	//�����ش���(PB�ж�0)
	//Ex_NVIC_Config(GPIO_C,0,RTIR); 	//�����ش���(PC�ж�0)
	//Ex_NVIC_Config(GPIO_D,0,RTIR); 	//�����ش���(PD�ж�0)
	//Ex_NVIC_Config(GPIO_E,0,RTIR); 	//�����ش���(PE�ж�0)
	
	//Ex_NVIC_Config(GPIO_A,0,FTIR); 	//�½��ش���(PE�ж�1)
	
	//Ex_NVIC_Config(GPIO_E,2,FTIR); 	//�½��ش���(PE�ж�2)
	
	//Ex_NVIC_Config(GPIO_E,3,FTIR); 	//�½��ش���(PE�ж�3)
	
 	//Ex_NVIC_Config(GPIO_E,4,FTIR); 	//�½��ش���(PE�ж�4)
	
	//MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//��ռ2�������ȼ�3����2(��ֵԽС���ȼ�Խ��)
	//MY_NVIC_Init(3,3,EXTI1_IRQn,2);	//��ռ3�������ȼ�3����2
	//MY_NVIC_Init(2,2,EXTI2_IRQn,2);	//��ռ2�������ȼ�2����2   
	//MY_NVIC_Init(2,1,EXTI3_IRQn,2);	//��ռ2�������ȼ�1����2	   
	//MY_NVIC_Init(2,0,EXTI4_IRQn,2);	//��ռ2�������ȼ�0����2	   
}



