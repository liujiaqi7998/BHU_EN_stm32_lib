#include "qiArdlib.h"


void setup()
{
  // put your setup code here, to run once:

  Serial.begin(9600); //使用9600的波特率进行串口通讯
}

void loop()
{
  if (Serial.available() > 0)
  {
    char serialDate = Serial.read();
    Serial.print(serialDate);
  }
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
