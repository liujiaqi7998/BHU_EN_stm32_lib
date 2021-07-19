#ifndef _STM32_spi_H
#define _STM32_spi_H
#include "sys.h"
/***********************************
file：_STM32_spi_H
作者：廖凌峰
时间：2019/8/23
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
//SPI->CR1//
//BIDIMODE
#define SPI_Two_wire_bidirectional       0<<15    //选择“双线双向”模式
#define SPI_Single_wire_bidirectional    1<<15    //选择“单线双向”模式
//BIDIOE     和BIDIMODE位一起决定在“单线双向”模式下数据的输出方向   
#define SPI_Output_DISABLE               0<<14    //输出禁止(只收模式)
#define SPI_Output_ENABLE                1<<14    //输出使能(只发模式)
//CRCEN
#define SPI_CRC_DISABLE                  0<<13    //禁止CRC计算
#define SPI_CRC_ENABLE                   1<<13    //启动CRC计算
//CRCNEXT
#define SPI_CRC_FROM_Buffer              0<<12    //下一个发送的CRC值来自发送缓冲区
#define SPI_CRC_FROM_CRC                 1<<12    //下一个发送的CRC值来自发送CRC寄存器
//DFF
#define SPI_8_bit_Data                   0<<11    //使用8位数据帧格式进行发送/接收
#define SPI_16_bit_Data                  1<<11    //使用16位数据帧格式进行发送/接收
//RXONLY
#define SPI_full_duplex                  0<<10    //全双工模式
#define SPI_Prohibit_export              1<<10    //禁止输出(只接收)
//SSM
#define SPI_SSM_Close                    0<<9     //禁止软件从设备管理
#define SPI_SSM_Open                     1<<9     //启用软件从设备管理
//SSI
#define SPI_SSI_low_When_SSM_Open        0<<8     //NSS上的电平为0    主设备
#define SPI_SSI_high_When_SSM_Open       1<<8     //NSS上的电平为1
//LSBFIRST
#define SPI_MSB_First                    0<<7     //先发送MSB
#define SPI_LSB_First                    1<<7     //先发送LSB
//SPE
#define SPI_DISABLE                      0<<6     //禁止SPI设备
#define SPI_ENABLE                       1<<6     //开启SPI设备
//BR[2:0]
#define SPI_Divide_by_2                  0<<3     //系统时钟二分频
#define SPI_Divide_by_4                  1<<3     //系统时钟四分频
#define SPI_Divide_by_8                  2<<3     //系统时钟八分频
#define SPI_Divide_by_16                 3<<3     //系统时钟十六分频
#define SPI_Divide_by_32                 4<<3     //系统时钟三十二分频
#define SPI_Divide_by_64                 5<<3     //系统时钟六十四分频
#define SPI_Divide_by_128                6<<3     //系统时钟一百零八分频
#define SPI_Divide_by_256                7<<3     //系统时钟二百五十六分频
//MSTR
#define SPI_Slave                        0<<2     //配置为从设备
#define SPI_Master                       1<<2     //配置为主设备
//CPOL
#define SPI_Idle_SCK_0                   0<<1     //空闲状态时，SCK保持低电平
#define SPI_Idle_SCK_1                   1<<1     //空闲状态时，SCK保持高电平
//CPHA
#define SPI_First_clock_edge_sampling    0<<0     //数据采样从第一个时钟边沿开始
#define SPI_Second_clock_edge_sampling   1<<0     //数据采样从第二个时钟边沿开始


extern void SPI2_init(void);
extern void SPI2_SetSpeed(uint8_t SpeedSet);
extern uint8_t SPI2_ReadWriteByte(uint8_t TxData);



#endif






