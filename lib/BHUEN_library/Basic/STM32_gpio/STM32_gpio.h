#ifndef __STM32_gpio_H
#define __STM32_gpio_H
#include <stm32f10x.h>

typedef enum
{
    A0=0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,
    A8,    A9,  A10, A11, A12, A13, A14, A15,
            
    B0=16, B1,  B2,  B3,  B4,  B5,  B6,  B7,
    B8,    B9,  B10, B11, B12, B13, B14, B15,
		
		C0=32, C1,  C2,  C3,  C4,  C5,  C6,  C7,
    C8,    C9,  C10, C11, C12, C13, C14, C15,
	
		D0=48, D1,  D2,  D3,  D4,  D5,  D6,  D7,
    D8,    D9,  D10, D11, D12, D13, D14, D15,
	
		E0=64, E1,  E2,  E3,  E4,  E5,  E6,  E7,
    E8,    E9,  E10, E11, E12, E13, E14, E15,
}PIN_enum;

#define GPIO_OUT             3    //输出模式
#define GPIO_IN              0    //输入模式
#define Analog_input         0    //模拟输入
#define Floating_input       1    //浮空输入
#define Up_and_down_input    2    //上下拉输入
#define Push_pull_output     0    //推挽输出
#define Open_drain_output    1    //开漏输出
#define Multiplex_push_pull  2    //复用推挽
#define Multiplex_open_drain 3    //复用开漏

extern void gpio_init(PIN_enum pin,uint16_t GPIO_Speed,uint16_t GPIO_Mode);
extern GPIO_TypeDef* change_pin(PIN_enum pin);
extern uint8_t gpio_GetIO(PIN_enum pin);
extern void gpio_Set(PIN_enum pin,uint16_t GPIO_Speed,uint16_t GPIO_Mode);
extern void gpio_Setstate(PIN_enum pin, uint8_t state);
extern void gpio_SetIO(PIN_enum pin);
extern PIN_enum show_pin(char i,char j);

extern void GPIO_init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t GPIO_Mode,uint16_t GPIO_Speed);
extern void GPIO_init_PinAll(GPIO_TypeDef* GPIOx,uint16_t GPIO_Mode,uint16_t GPIO_Speed);
extern void GPIO_Set(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,uint16_t GPIO_Mode,uint16_t GPIO_Speed);
extern void GPIO_init_All_like(GPIO_TypeDef* GPIOx,uint16_t GPIO_Mode,uint16_t GPIO_Speed,uint16_t temp);
extern void GPIO_Setstate(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t state);
extern uint8_t GPIO_GetIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void GPIO_SetIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void GPIO_USART_IOint(USART_TypeDef * USARTx);
extern void GPIO_TIM_IOint(TIM_TypeDef * TIMx,uint8_t CHx);

#endif




