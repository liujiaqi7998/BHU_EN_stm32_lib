#ifndef _STM32_spi_H
#define _STM32_spi_H
#include "sys.h"
/***********************************
file��_STM32_spi_H
���ߣ������
ʱ�䣺2019/8/23
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
//SPI->CR1//
//BIDIMODE
#define SPI_Two_wire_bidirectional       0<<15    //ѡ��˫��˫��ģʽ
#define SPI_Single_wire_bidirectional    1<<15    //ѡ�񡰵���˫��ģʽ
//BIDIOE     ��BIDIMODEλһ������ڡ�����˫��ģʽ�����ݵ��������   
#define SPI_Output_DISABLE               0<<14    //�����ֹ(ֻ��ģʽ)
#define SPI_Output_ENABLE                1<<14    //���ʹ��(ֻ��ģʽ)
//CRCEN
#define SPI_CRC_DISABLE                  0<<13    //��ֹCRC����
#define SPI_CRC_ENABLE                   1<<13    //����CRC����
//CRCNEXT
#define SPI_CRC_FROM_Buffer              0<<12    //��һ�����͵�CRCֵ���Է��ͻ�����
#define SPI_CRC_FROM_CRC                 1<<12    //��һ�����͵�CRCֵ���Է���CRC�Ĵ���
//DFF
#define SPI_8_bit_Data                   0<<11    //ʹ��8λ����֡��ʽ���з���/����
#define SPI_16_bit_Data                  1<<11    //ʹ��16λ����֡��ʽ���з���/����
//RXONLY
#define SPI_full_duplex                  0<<10    //ȫ˫��ģʽ
#define SPI_Prohibit_export              1<<10    //��ֹ���(ֻ����)
//SSM
#define SPI_SSM_Close                    0<<9     //��ֹ������豸����
#define SPI_SSM_Open                     1<<9     //����������豸����
//SSI
#define SPI_SSI_low_When_SSM_Open        0<<8     //NSS�ϵĵ�ƽΪ0    ���豸
#define SPI_SSI_high_When_SSM_Open       1<<8     //NSS�ϵĵ�ƽΪ1
//LSBFIRST
#define SPI_MSB_First                    0<<7     //�ȷ���MSB
#define SPI_LSB_First                    1<<7     //�ȷ���LSB
//SPE
#define SPI_DISABLE                      0<<6     //��ֹSPI�豸
#define SPI_ENABLE                       1<<6     //����SPI�豸
//BR[2:0]
#define SPI_Divide_by_2                  0<<3     //ϵͳʱ�Ӷ���Ƶ
#define SPI_Divide_by_4                  1<<3     //ϵͳʱ���ķ�Ƶ
#define SPI_Divide_by_8                  2<<3     //ϵͳʱ�Ӱ˷�Ƶ
#define SPI_Divide_by_16                 3<<3     //ϵͳʱ��ʮ����Ƶ
#define SPI_Divide_by_32                 4<<3     //ϵͳʱ����ʮ����Ƶ
#define SPI_Divide_by_64                 5<<3     //ϵͳʱ����ʮ�ķ�Ƶ
#define SPI_Divide_by_128                6<<3     //ϵͳʱ��һ����˷�Ƶ
#define SPI_Divide_by_256                7<<3     //ϵͳʱ�Ӷ�����ʮ����Ƶ
//MSTR
#define SPI_Slave                        0<<2     //����Ϊ���豸
#define SPI_Master                       1<<2     //����Ϊ���豸
//CPOL
#define SPI_Idle_SCK_0                   0<<1     //����״̬ʱ��SCK���ֵ͵�ƽ
#define SPI_Idle_SCK_1                   1<<1     //����״̬ʱ��SCK���ָߵ�ƽ
//CPHA
#define SPI_First_clock_edge_sampling    0<<0     //���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
#define SPI_Second_clock_edge_sampling   1<<0     //���ݲ����ӵڶ���ʱ�ӱ��ؿ�ʼ


extern void SPI2_init(void);
extern void SPI2_SetSpeed(uint8_t SpeedSet);
extern uint8_t SPI2_ReadWriteByte(uint8_t TxData);



#endif






