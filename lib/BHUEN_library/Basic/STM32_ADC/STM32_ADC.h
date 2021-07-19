#ifndef STM32_ADC_H
#define STM32_ADC_H
#include "STM32_gpio.h"
#include "STM32_usart.h"
void ADC1_init(PIN_enum pin);
void ADC2_init(PIN_enum pin);
u16 get_ADC1(PIN_enum pin);
u16 get_ADC2(PIN_enum pin);
void show_ADC1(USART_TypeDef * USARTx,PIN_enum pin);
void show_ADC2(USART_TypeDef * USARTx,PIN_enum pin);


#endif
