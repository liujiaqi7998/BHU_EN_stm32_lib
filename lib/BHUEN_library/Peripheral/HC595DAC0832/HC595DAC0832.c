#include "STM32_head.h"
#include "HC595DAC0832.h"
/***********************************
file：HC595DAC0832.c
作者：蔡晓锦
时间：2019/7/17
本代码仅供北华大学嵌入式系统实验室使用
***********************************/

/*HC595引脚定义*/
#define MOSIO A1  //DS:PA1;
#define R_CLK A2  //ST_CP:PA2;
#define S_CLK A3  //SH_CP:PA3;

/*DAC0832引脚定义*/
#define CSDA A5//PA5
#define WR   A6//PA6

char  Sin[64]={
135,145,158,167,176,188,199,209,218,226,234,240,245,249,252,254,254,253,251,247,243,237,230,222,213,204,193,182,170,158,
146,133,121,108,96,84,72,61,50,41,32,24,17,11,7,3,1,0,0,2,5,9,14,20,28,36,45,55,66,78,90,102,114,128
};  //正弦波

char  rectangle[64]={
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};  //矩形波
char  sawtooth[64]={
0,4,8,12,16,20,24,28,32,36,40,45,49,53,57,61,65,69,73,77,81,85,89,93,97,101,105,109,113,117,121,125,130,134,138,142,
146,150,154,158,162,166,170,174,178,182,186,190,194,198,202,206,210,215,219,223,227,231,235,239,243,247,251,255
};  //锯齿波
char  Trapezoid[64]={
0,13,26,39,52,65,78,91,104,117,130,143,156,169,182,195,208,221,234,247,247,247,247,247,247,
247,247,247,247,247,247,247,247,247,247,247,247,247,
247,247,247,247,247,247,247,242,229,216,203,190,177,164,151,138,125,112,99,86,73,60,47,34,21,8
};  //梯形波
char  triangle[64]={
0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,152,160,168,176,184,192,200,208
,216,224,232,240,248,
248,240,232,224,216,208,200,192,184,176,168,160,152,144,136,128,120,112,104,96,88,80,72,64,
56,48,40,32,24,16,8,0
};  //三角波
 
void NOP(void)
{
  uint16_t i;
  for (i = 0; i < 20; i++);
}
/************************************
函数名:HC595SendData(uint8_t SendVal)
描述：595发送数据
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
函数名:HC595GPIO_init
描述：初始化HC595端口(可对文件上方宏定义进行直接修改)
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
函数名:DAC0832GPIO_init
描述：初始化DAC0832GPIO端口(可对文件上方宏定义进行直接修改)
***********************************/
void DAC0832GPIO_init(){
	//GPIO_ResetBits(GPIOA, CSDA);//拉低CSDA
	gpio_Setstate(CSDA,0);
	//GPIO_ResetBits(GPIOA, WR);//拉低WR
	gpio_Setstate(WR,0);
}

/************************************
函数名:demo测试程序
描述：单片机输出串行数据，595转换成并行数据之后发送给DAC0832进行输出(注：DAC0832输出的是电流，加运放转换后才是电压)
***********************************/
void  demo()
{
	int k=0;
	HC595GPIO_init();
	DAC0832GPIO_init();
	while(1)
	{
			HC595SendData(rectangle[k]);//需要的波形自选
			k++;
			if(k==64)
				k=0;
	}
}
