#include "STM32_spi.h"
#include <stm32f10x.h>
#include "sys.h"
/***********************************
file��STM32_spi.c
���ߣ������
ʱ�䣺2019/8/23
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
void SPI2_init(void)
{
	RCC->APB2ENR|=1<<3;//PORTB ʱ��ʹ��
	RCC->APB1ENR|=1<<14; //SPI2 ʱ��ʹ��
	//����ֻ��� SPI �ڳ�ʼ��
	GPIOB->CRH&=0X000FFFFF;
	GPIOB->CRH|=0XBBB00000;  //PB13/14/15 ����
	GPIOB->ODR|=0x7<<13; //PB13/14/15 ����
	SPI2->CR1|=SPI_full_duplex;  //ȫ˫��ģʽ
	SPI2->CR1|=SPI_SSM_Open; //��� nss ����
	SPI2->CR1|=SPI_SSI_high_When_SSM_Open;//NSS����
	SPI2->CR1|=SPI_Master; //SPI ����
	SPI2->CR1|=SPI_8_bit_Data;  //8bit ���ݸ�ʽ 
	SPI2->CR1|=SPI_Idle_SCK_1; //����ģʽ�� SCK Ϊ 1 CPOL=1
	SPI2->CR1|=SPI_Second_clock_edge_sampling; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1
	//�� SPI2 ���� APB1 ������.ʱ��Ƶ�����Ϊ 36M.
	SPI2->CR1|=SPI_Divide_by_256; //Fsck=Fpclk1/256
	SPI2->CR1|=SPI_MSB_First; //MSBfirst
	SPI2->CR1|=SPI_ENABLE; //SPI �豸ʹ��
	
	//SPI2_ReadWriteByte(0xff); //�������� 
}
//SPI2 �ٶ����ú���
//SpeedSet:0~7
//SPI �ٶ�=fAPB1/2^(SpeedSet+1)
//APB1 ʱ��һ��Ϊ 36Mhz
void SPI2_SetSpeed(uint8_t SpeedSet)
{
	SpeedSet&=0X07; //���Ʒ�Χ
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SpeedSet<<3; //���� SPI2 �ٶ�
	SPI2->CR1|=SPI_ENABLE; //SPI �豸ʹ�� 
}
//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{ 
	uint16_t retry=0;
	while((SPI2->SR&1<<1)==0)  //�ȴ��������� 
	{
		retry++;
		if(retry>=0XFFFE)return 0; //��ʱ�˳�
	} 
	SPI2->DR=TxData;  //����һ�� byte
	retry=0;
	while((SPI2->SR&1<<0)==0) //�ȴ�������һ�� byte
	{
		retry++;
		if(retry>=0XFFFE)return 0; //��ʱ�˳�
	}
	return SPI2->DR; //�����յ�������
} 








