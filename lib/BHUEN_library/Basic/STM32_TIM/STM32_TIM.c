#include "STM32_head.h"
#include "STM32_TIM.h"
/***********************************
file��STM32_TIM.c
���ߣ������
ʱ�䣺2019/7/16
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
//��ʱ��1   72MHZ
//           TIM1_CH1----PA8       //
//           TIM1_CH2----PA9       //
//           TIM1_CH3----PA10      //
//           TIM1_CH4----PA11      //
//��ʱ��2   36MHZ
//           TIM2_CH1----PA0       //
//           TIM2_CH2----PA1       //
//           TIM2_CH3----PA2       //
//           TIM2_CH4----PA3       //
//��ʱ��3   36MHZ
//           TIM3_CH1----PA6       //
//           TIM3_CH2----PA7       //
//           TIM3_CH3----PB0       //
//           TIM3_CH4----PB1       //
//��ʱ��4   36MHZ
//           TIM4_CH1----PB6       //
//           TIM4_CH2----PB7       //
//           TIM4_CH3----PB8       //
//           TIM4_CH4----PB9       //
//��ʱ��5   36MHZ
//           TIM5_CH1----PA0       //
//           TIM5_CH2----PA1       //
//           TIM5_CH3----PA2       //
//           TIM5_CH4----PA3       //

/************************************
������:TIM_PWM_init(ֻ�ܳ�ʼ��TIM1��TIM2��TIM3��TIM4��TIM5)
����:��ʱ��pwm��ʼ������ʹ����ӳ�䣩
���룺TIMx(��ʱ����),CHx(ͨ����),psc(Ԥ��Ƶϵ��),arr(����ʱ��)
�������
������TIM_PWM_init(TIM3,3,72,20000);//TIM3ͨ��3,10KHZ,20ms,
***********************************/
void TIM_PWM_init(TIM_TypeDef * TIMx,uint8_t CHx,uint32_t psc,uint32_t arr)
{
	GPIO_TIM_IOint(TIMx,CHx);
	TIMx->PSC=psc-1;//  psc=72   72000000/((PSC+1)*ARR)=50HZ 0.001ms 1us
	TIMx->ARR=arr-1;//
	if(CHx==1)
	{
		TIMx->CCR1=(arr/2)-1;//ͨ��1�����
		TIMx->CCER|=(1<<0);//�������Ӧ����
		TIMx->CCMR1|=(6<<4);//pwmģʽ1���ϼ���,�ȸ�
		TIMx->CCMR1|=(1<<3);//Ԥװ��ʹ��
	}
	else if(CHx==2)
	{
		TIMx->CCR2=(arr/2)-1;//ͨ��2�����
		TIMx->CCER|=(1<<4);//�������Ӧ����
		TIMx->CCMR1|=(6<<12);//pwmģʽ1���ϼ���,�ȸ�
		TIMx->CCMR1|=(1<<11);//Ԥװ��ʹ��
	}
	else if(CHx==3)
	{
		TIMx->CCR3=(arr/2)-1;//ͨ��3�����
		TIMx->CCER|=(1<<8);//�������Ӧ����
		TIMx->CCMR2|=(6<<4);//pwmģʽ1���ϼ���,�ȸ�
		TIMx->CCMR2|=(1<<3);//Ԥװ��ʹ��
	}
	else if(CHx==4)
	{
		TIMx->CCR4=(arr/2)-1;//ͨ��4�����
		TIMx->CCER|=(1<<12);//�������Ӧ����
		TIMx->CCMR2|=(6<<12);//pwmģʽ1���ϼ���,�ȸ�
		TIMx->CCMR2|=(1<<11);//Ԥװ��ʹ��
	}
	TIMx->CR1=0x0080; //ARPEʹ�ܣ�������װ������λ��
	TIMx->CR1|=0x01;//ʹ�ܶ�ʱ��
	if(TIMx==TIM1||TIMx==TIM8)//�߼���ʱ����Ҫ
	{
		TIMx->BDTR|=(1<<14);
	}
}
/************************************
������:PWM_change_CCR1
����:PWM�ı������
���룺TIMx(��ʱ����),ccr1(�����)
�������
������PWM_change_CCRx(TIM3,3,1500);//TIM3����1500������
***********************************/
void PWM_change_CCRx(TIM_TypeDef * TIMx,uint8_t CHx,uint16_t ccr1)
{
	if(CHx==1)
		TIMx->CCR1=ccr1-1;//�����
	else if(CHx==2)
		TIMx->CCR2=ccr1-1;//�����
	else if(CHx==3)
		TIMx->CCR3=ccr1-1;//�����
	else if(CHx==4)
		TIMx->CCR4=ccr1-1;//�����
}
/************************************
������:TIMx_irq_init(�ж����ȼ�Ҫ�ڱ��������һ���޸�)    
����:��ʱ���жϳ�ʼ��                                   //->   �жϴ�����λ��  "irq.c"    <-//
���룺TIMx(��ʱ����),arr(�Զ�װ��ֵ),psc(��Ƶϵ��)
�������
������TIMx_irq_init(TIM3,10000,7200);//TIM3 ��ʱ������ʱ�� 1S    ʱ��Ƶ�� 10khz 
***********************************/
void TIMx_irq_init(TIM_TypeDef * TIMx,u16 arr,u16 psc)
{
	if(TIMx==TIM1)
		RCC->APB2ENR|=1<<11;//ʹ�ܶ�ʱ��1ʱ��
	else if(TIMx==TIM2)
		RCC->APB1ENR|=1<<0;//ʹ�ܶ�ʱ��2ʱ��
	else if(TIMx==TIM3)
		RCC->APB1ENR|=1<<1;//ʹ�ܶ�ʱ��3ʱ��
	else if(TIMx==TIM4)
		RCC->APB1ENR|=1<<2;//ʹ�ܶ�ʱ��4ʱ��
	else if(TIMx==TIM5)
		RCC->APB1ENR|=1<<3;//ʹ�ܶ�ʱ��5ʱ��
	
	TIMx->ARR=arr-1;//�����Զ�װ��ֵ    ʱ��=ʱ��Ƶ��*arr
	TIMx->PSC=psc-1;//ʱ��Ƶ��=1/(72000000/psc)
	TIMx->DIER|=1<<0;//�����жϸ���
	TIMx->CR1|=0x01;//ʹ�ܼ�ʱ�����ڲ�rc��
	
	// if(TIMx==TIM2)
	// 	MY_NVIC_Init(2,2,TIM2_IRQn,2);//(���ȼ������޸�)
	// else if(TIMx==TIM3)
	// 	MY_NVIC_Init(2,2,TIM3_IRQn,2);
	// else if(TIMx==TIM4)
	// 	MY_NVIC_Init(2,2,TIM4_IRQn,2);
	//else if(TIMx==TIM5)   //STM32f103C8T6û�ж�ʱ��5,��Ҫ����
	//	MY_NVIC_Init(2,2,TIM5_IRQn,2);
}
/************************************
������:TIMx_irq_close  
����:��ʱ���жϹر�                                  //->   �жϴ�����λ��  "irq.c"    <-//
���룺TIMx(��ʱ����)
�������
������TIMx_irq_close(TIM3);//TIM3 ��ʱ���ر�
***********************************/
void TIMx_irq_close(TIM_TypeDef * TIMx)
{
	TIMx->DIER&=~(1<<0);//�������жϸ���
	TIMx->CR1&=~(1<<0);//��ֹʹ�ܼ�ʱ�����ڲ�rc��
}

