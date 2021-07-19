#ifndef __STM32_TIM_H
#define __STM32_TIM_H
#include <stm32f10x.h>


extern void TIM_PWM_init(TIM_TypeDef * TIMx,uint8_t CHx,uint32_t psc,uint32_t arr);
extern void PWM_change_CCRx(TIM_TypeDef * TIMx,uint8_t CHx,uint16_t ccr1);
extern void TIMx_irq_init(TIM_TypeDef * TIMx,u16 arr,u16 psc);
extern void TIMx_irq_close(TIM_TypeDef * TIMx);


#endif


