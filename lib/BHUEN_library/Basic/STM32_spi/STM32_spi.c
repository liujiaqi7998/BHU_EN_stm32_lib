#include "STM32_spi.h"
#include <stm32f10x.h>
#include "sys.h"
/***********************************
file：STM32_spi.c
作者：廖凌峰
时间：2019/8/23
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
void SPI2_init(void)
{
	RCC->APB2ENR|=1<<3;//PORTB 时钟使能
	RCC->APB1ENR|=1<<14; //SPI2 时钟使能
	//这里只针对 SPI 口初始化
	GPIOB->CRH&=0X000FFFFF;
	GPIOB->CRH|=0XBBB00000;  //PB13/14/15 复用
	GPIOB->ODR|=0x7<<13; //PB13/14/15 上拉
	SPI2->CR1|=SPI_full_duplex;  //全双工模式
	SPI2->CR1|=SPI_SSM_Open; //软件 nss 管理
	SPI2->CR1|=SPI_SSI_high_When_SSM_Open;//NSS控制
	SPI2->CR1|=SPI_Master; //SPI 主机
	SPI2->CR1|=SPI_8_bit_Data;  //8bit 数据格式 
	SPI2->CR1|=SPI_Idle_SCK_1; //空闲模式下 SCK 为 1 CPOL=1
	SPI2->CR1|=SPI_Second_clock_edge_sampling; //数据采样从第二个时间边沿开始,CPHA=1
	//对 SPI2 属于 APB1 的外设.时钟频率最大为 36M.
	SPI2->CR1|=SPI_Divide_by_256; //Fsck=Fpclk1/256
	SPI2->CR1|=SPI_MSB_First; //MSBfirst
	SPI2->CR1|=SPI_ENABLE; //SPI 设备使能
	
	//SPI2_ReadWriteByte(0xff); //启动传输 
}
//SPI2 速度设置函数
//SpeedSet:0~7
//SPI 速度=fAPB1/2^(SpeedSet+1)
//APB1 时钟一般为 36Mhz
void SPI2_SetSpeed(uint8_t SpeedSet)
{
	SpeedSet&=0X07; //限制范围
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SpeedSet<<3; //设置 SPI2 速度
	SPI2->CR1|=SPI_ENABLE; //SPI 设备使能 
}
//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{ 
	uint16_t retry=0;
	while((SPI2->SR&1<<1)==0)  //等待发送区空 
	{
		retry++;
		if(retry>=0XFFFE)return 0; //超时退出
	} 
	SPI2->DR=TxData;  //发送一个 byte
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个 byte
	{
		retry++;
		if(retry>=0XFFFE)return 0; //超时退出
	}
	return SPI2->DR; //返回收到的数据
} 








