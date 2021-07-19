#include "dht11.h"
#include "STM32_head.h"
/***********************************
file��dht11.c
���ߣ�������
ʱ�䣺2019/7/17
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
#define DHT11_pin A11   //�����޸�
/************************************
ʹ�÷����� ���ţ�A11 ������� ����ת�� DHT11_Init()���� 
 
	DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ					    
	printf("�¶�:%d\t",temperature);			//�������
	printf("ʪ��:%d\n",humidity);	
	
*************************************/

/************************************
������:DHT11_Read_Data
��������ȡDHT11����
���룺��
���������ֵ
������DHT11_Read_Data(&�¶�, &ʪ��);
***********************************/

u8 DHT11_Read_Data(u8 *temp,u8 *humi) {        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0) {
		for(i=0;i<5;i++) {
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4]) {
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}


/************************************
(ϵͳ�Զ����ã��û�����Ҫ�鿴)
***********************************/

void DHT11_Rst(void) {                 
	//DHT11_IO_OUT(); 	
	gpio_Set(DHT11_pin,3,0);
    //DHT11_DQ_OUT=0; 
	gpio_Setstate(DHT11_pin,0);
    delay_ms(20);    	
    //DHT11_DQ_OUT=1; 	
	gpio_Setstate(DHT11_pin,1);
	delay_us(30);     
}

u8 DHT11_Check(void) {   
	u8 retry=0;
	//DHT11_IO_IN(); 
	gpio_Set(DHT11_pin,0,2);
    //while (DHT11_DQ_IN&&retry<100) {
		while (gpio_GetIO(DHT11_pin)&&retry<100) {
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    //while (!DHT11_DQ_IN&&retry<100)	{
		while (!gpio_GetIO(DHT11_pin)&&retry<100)	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

u8 DHT11_Read_Bit(void) {
 	u8 retry=0;
	//while(DHT11_DQ_IN&&retry<100) {
	while(gpio_GetIO(DHT11_pin)&&retry<100) {
		retry++;
		delay_us(1);
	}
	retry=0;
	//while(!DHT11_DQ_IN&&retry<100) {
	while(!gpio_GetIO(DHT11_pin)&&retry<100) {
		retry++;
		delay_us(1);
	}
	delay_us(40);
	//if(DHT11_DQ_IN)return 1;
	if(gpio_GetIO(DHT11_pin))return 1;
	else return 0;		   
}

u8 DHT11_Read_Byte(void) {        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) {
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
  

u8 DHT11_Init(void) { 
	//RCC->APB2ENR|=1<<2;    
	
	//GPIOA->CRH&=0XFFFF0FFF;
	//GPIOA->CRH|=0X00003000;
	//GPIOA->ODR|=1<<11;  
	gpio_init(DHT11_pin,3,0);	
	DHT11_Rst();
	return DHT11_Check();
}























