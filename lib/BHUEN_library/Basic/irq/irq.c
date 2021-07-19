#include "STM32_head.h"
#include "irq.h"
#include "delay.h"
/***********************************
file：irq.c
作者：廖凌峰
时间：2019/7/16
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
//***********************定时器中断***********************************//

//定时器中断2服务程序
// extern unsigned int Timer2_Counter;
// extern unsigned int W5500_Send_Delay_Counter;
// void TIM2_IRQHandler(void)
// {
// 	if((TIM2->SR&0x0001))//发生中断
// 	{
// 		//gpio_SetIO(A6);
// 		Timer2_Counter++;
// 		W5500_Send_Delay_Counter++;
// 	}
// 	TIM2->SR&=~(1<<0);//清除中断标记
// }

// //定时器中断3服务程序
// void TIM3_IRQHandler(void)
// {
// 	if((TIM3->SR&0x0001))//发生中断
// 	{
// 		//gpio_SetIO(A6);
// 	}
// 	TIM3->SR&=~(1<<0);//清除中断标记
// }

// //定时器中断4服务程序
// //void TIM4_IRQHandler(void)
// //{
// //	if((TIM4->SR&0x0001))//发生中断
// //	{
// //		//gpio_SetIO(A6);
// //	}
// //	TIM4->SR&=~(1<<0);//清除中断标记
// //}

// //定时器中断5服务程序
// void TIM5_IRQHandler(void)
// {
// 	if((TIM5->SR&0x0001))//发生中断
// 	{
// 		gpio_SetIO(A6);
// 		TIMx_irq_close(TIM5);
// 	}
// 	TIM5->SR&=~(1<<0);//清除中断标记
// }
// //******************************************************************//







// //***********************外部中断***********************************//

// //外部中断0服务程序
// void EXTI0_IRQHandler(void)
// {
// //	gpio_SetIO(A6);
// //	delay_ms(1000);//消抖
// 	gpio_SetIO(B5);

// 	EXTI->PR=1<<0; //清除LINE0上的中断标志位
// }

// //外部中断1服务程序
// void EXTI1_IRQHandler(void)
// {
// 		gpio_SetIO(B5);

// 	EXTI->PR=1<<1; //清除LINE1上的中断标志位
// }

// //外部中断2服务程序
// void EXTI2_IRQHandler(void)
// {
// 	gpio_SetIO(A6);
// 	delay_ms(1000);//消抖
// 	EXTI->PR=1<<2; //清除LINE2上的中断标志位
// }

// //外部中断3服务程序
// void EXTI3_IRQHandler(void)
// {	
// 	gpio_SetIO(A6);
// 	delay_ms(1000);//消抖
// 	EXTI->PR=1<<3; //清除LINE3上的中断标志位
// }

// //外部中断4服务程序
// void EXTI4_IRQHandler(void)
// {
// 	gpio_SetIO(A6);
// 	delay_ms(1000);//消抖
// 	EXTI->PR=1<<4; //清除LINE4上的中断标志位
// }
// //******************************************************************//

// //***********************串口中断***********************************//
// extern u16 USART_RX_STA;
// void USART2_IRQHandler(void)
// {
// 	u8 res;
// 	if(USART2->SR&(1<<5))	//接收到数据
// 	{	 
// 		res=USART2->DR; 
// 		if((USART_RX_STA&0x8000)==0)//接收未完成
// 		{
// 			if(USART_RX_STA&0x4000)//接收到了0x0d
// 			{
// 				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
// 				else USART_RX_STA|=0x8000;	//接收完成了 
// 			}else //还没收到0X0D
// 			{	
// 				if(res==0x0d)USART_RX_STA|=0x4000;
// 				else
// 				{
// 					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
// 					USART_RX_STA++;
// 					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
// 				}		 
// 			}
// 		}  		 									     
// 	}
// }
