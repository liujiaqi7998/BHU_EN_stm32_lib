#include "qiArdlib.h"
#define ADC_Pin PA0

void setup()
{
  // put your setup code here, to run once:
  ADCx_Init(ADC1);
  Serial.begin(9600); //使用9600的波特率进行串口通讯
  Serial.printf("hello");
  pinMode(ADC_Pin, INPUT_ANALOG);
}

void loop()
{
    uint16_t ADC_Val = analogRead(ADC_Pin);
    Serial.printf("ADC_Pin ADC Val is:%d\r\n", ADC_Val);
    delay(1000);
}

/**
  * @brief  Main Function
  * @param  None
  * @retval None
  */
int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  SysClock_Init(F_CPU_72MHz);
  Delay_Init();
  setup();
  for (;;)
    loop();
}
