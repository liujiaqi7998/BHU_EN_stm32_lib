#include "STM32_head.h"
#include "irq.h"
#include "delay.h"
/***********************************
file��irq.c
���ߣ������
ʱ�䣺2019/7/16
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
//***********************��ʱ���ж�***********************************//

//��ʱ���ж�2�������
// extern unsigned int Timer2_Counter;
// extern unsigned int W5500_Send_Delay_Counter;
// void TIM2_IRQHandler(void)
// {
// 	if((TIM2->SR&0x0001))//�����ж�
// 	{
// 		//gpio_SetIO(A6);
// 		Timer2_Counter++;
// 		W5500_Send_Delay_Counter++;
// 	}
// 	TIM2->SR&=~(1<<0);//����жϱ��
// }

// //��ʱ���ж�3�������
// void TIM3_IRQHandler(void)
// {
// 	if((TIM3->SR&0x0001))//�����ж�
// 	{
// 		//gpio_SetIO(A6);
// 	}
// 	TIM3->SR&=~(1<<0);//����жϱ��
// }

// //��ʱ���ж�4�������
// //void TIM4_IRQHandler(void)
// //{
// //	if((TIM4->SR&0x0001))//�����ж�
// //	{
// //		//gpio_SetIO(A6);
// //	}
// //	TIM4->SR&=~(1<<0);//����жϱ��
// //}

// //��ʱ���ж�5�������
// void TIM5_IRQHandler(void)
// {
// 	if((TIM5->SR&0x0001))//�����ж�
// 	{
// 		gpio_SetIO(A6);
// 		TIMx_irq_close(TIM5);
// 	}
// 	TIM5->SR&=~(1<<0);//����жϱ��
// }
// //******************************************************************//







// //***********************�ⲿ�ж�***********************************//

// //�ⲿ�ж�0�������
// void EXTI0_IRQHandler(void)
// {
// //	gpio_SetIO(A6);
// //	delay_ms(1000);//����
// 	gpio_SetIO(B5);

// 	EXTI->PR=1<<0; //���LINE0�ϵ��жϱ�־λ
// }

// //�ⲿ�ж�1�������
// void EXTI1_IRQHandler(void)
// {
// 		gpio_SetIO(B5);

// 	EXTI->PR=1<<1; //���LINE1�ϵ��жϱ�־λ
// }

// //�ⲿ�ж�2�������
// void EXTI2_IRQHandler(void)
// {
// 	gpio_SetIO(A6);
// 	delay_ms(1000);//����
// 	EXTI->PR=1<<2; //���LINE2�ϵ��жϱ�־λ
// }

// //�ⲿ�ж�3�������
// void EXTI3_IRQHandler(void)
// {	
// 	gpio_SetIO(A6);
// 	delay_ms(1000);//����
// 	EXTI->PR=1<<3; //���LINE3�ϵ��жϱ�־λ
// }

// //�ⲿ�ж�4�������
// void EXTI4_IRQHandler(void)
// {
// 	gpio_SetIO(A6);
// 	delay_ms(1000);//����
// 	EXTI->PR=1<<4; //���LINE4�ϵ��жϱ�־λ
// }
// //******************************************************************//

// //***********************�����ж�***********************************//
// extern u16 USART_RX_STA;
// void USART2_IRQHandler(void)
// {
// 	u8 res;
// 	if(USART2->SR&(1<<5))	//���յ�����
// 	{	 
// 		res=USART2->DR; 
// 		if((USART_RX_STA&0x8000)==0)//����δ���
// 		{
// 			if(USART_RX_STA&0x4000)//���յ���0x0d
// 			{
// 				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
// 				else USART_RX_STA|=0x8000;	//��������� 
// 			}else //��û�յ�0X0D
// 			{	
// 				if(res==0x0d)USART_RX_STA|=0x4000;
// 				else
// 				{
// 					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
// 					USART_RX_STA++;
// 					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
// 				}		 
// 			}
// 		}  		 									     
// 	}
// }
