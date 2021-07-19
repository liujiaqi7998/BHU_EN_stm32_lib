#include "STM32_head.h"
#include "HC595DAC0832.h"
/***********************************
file��HC595DAC0832.c
���ߣ�������
ʱ�䣺2019/7/17
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/

/*HC595���Ŷ���*/
#define MOSIO A1  //DS:PA1;
#define R_CLK A2  //ST_CP:PA2;
#define S_CLK A3  //SH_CP:PA3;

/*DAC0832���Ŷ���*/
#define CSDA A5//PA5
#define WR   A6//PA6

char  Sin[64]={
135,145,158,167,176,188,199,209,218,226,234,240,245,249,252,254,254,253,251,247,243,237,230,222,213,204,193,182,170,158,
146,133,121,108,96,84,72,61,50,41,32,24,17,11,7,3,1,0,0,2,5,9,14,20,28,36,45,55,66,78,90,102,114,128
};  //���Ҳ�

char  rectangle[64]={
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};  //���β�
char  sawtooth[64]={
0,4,8,12,16,20,24,28,32,36,40,45,49,53,57,61,65,69,73,77,81,85,89,93,97,101,105,109,113,117,121,125,130,134,138,142,
146,150,154,158,162,166,170,174,178,182,186,190,194,198,202,206,210,215,219,223,227,231,235,239,243,247,251,255
};  //��ݲ�
char  Trapezoid[64]={
0,13,26,39,52,65,78,91,104,117,130,143,156,169,182,195,208,221,234,247,247,247,247,247,247,
247,247,247,247,247,247,247,247,247,247,247,247,247,
247,247,247,247,247,247,247,242,229,216,203,190,177,164,151,138,125,112,99,86,73,60,47,34,21,8
};  //���β�
char  triangle[64]={
0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,152,160,168,176,184,192,200,208
,216,224,232,240,248,
248,240,232,224,216,208,200,192,184,176,168,160,152,144,136,128,120,112,104,96,88,80,72,64,
56,48,40,32,24,16,8,0
};  //���ǲ�
 
void NOP(void)
{
  uint16_t i;
  for (i = 0; i < 20; i++);
}
/************************************
������:HC595SendData(uint8_t SendVal)
������595��������
***********************************/
void HC595SendData(unsigned char SendVal)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    if (((SendVal << i) & 0x80) != 0 )
				//GPIO_SetBits(GPIOA, MOSIO);
				gpio_Setstate(MOSIO,1);
    else 
				//GPIO_ResetBits(GPIOA, MOSIO);
				gpio_Setstate(MOSIO,0);
 
    //GPIO_ResetBits(GPIOA, S_CLK);
		gpio_Setstate(S_CLK,0);
    NOP();
    //GPIO_SetBits(GPIOA, S_CLK);
		gpio_Setstate(S_CLK,1);
  }
  //GPIO_ResetBits(GPIOA, R_CLK);
	gpio_Setstate(R_CLK,0);
  NOP();
  //GPIO_SetBits(GPIOA, R_CLK);
	gpio_Setstate(R_CLK,1);
}
/************************************
������:HC595GPIO_init
��������ʼ��HC595�˿�(�ɶ��ļ��Ϸ��궨�����ֱ���޸�)
***********************************/
void HC595GPIO_init(){
	
  //GPIO_InitTypeDef GPIO_InitS;
 
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

  //GPIO_InitS.GPIO_Pin = GPIO_Pin_All;
  //GPIO_InitS.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_InitS.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_Init(GPIOA, &GPIO_InitS);
  //GPIO_Write(GPIOA, 0xffff);
	gpio_init(MOSIO,3,0);
	gpio_init(R_CLK,3,0);
	gpio_init(S_CLK,3,0);
	gpio_init(CSDA,3,0);
	gpio_init(WR,3,0);
	
}
/************************************
������:DAC0832GPIO_init
��������ʼ��DAC0832GPIO�˿�(�ɶ��ļ��Ϸ��궨�����ֱ���޸�)
***********************************/
void DAC0832GPIO_init(){
	//GPIO_ResetBits(GPIOA, CSDA);//����CSDA
	gpio_Setstate(CSDA,0);
	//GPIO_ResetBits(GPIOA, WR);//����WR
	gpio_Setstate(WR,0);
}

/************************************
������:demo���Գ���
��������Ƭ������������ݣ�595ת���ɲ�������֮���͸�DAC0832�������(ע��DAC0832������ǵ��������˷�ת������ǵ�ѹ)
***********************************/
void  demo()
{
	int k=0;
	HC595GPIO_init();
	DAC0832GPIO_init();
	while(1)
	{
			HC595SendData(rectangle[k]);//��Ҫ�Ĳ�����ѡ
			k++;
			if(k==64)
				k=0;
	}
}
