#include "NRF24L01.h"
#include "STM32_head.h"
//��ֲ�������
//��ֲʱ�䣺2019/8/23
//��ֲ���Գɶ���ɿƼ����޹�˾LPC54606J512BD100������


//     ���ƣ������   
//     NRF24L01����ͼ    
//    . GND   . 3.3V                         |����
//    . CE    . CS                           |����                    
//    . SCK   . MOSI                         |����
//    . MISO  . IRQ                          |����

#define NRF_CE(x)   gpio_Setstate(NRF_CE_PIN,x)
#define NRF_CSN(x)  gpio_Setstate(NRF_CSN_PIN,x)
#define NRF_MOSI(x) gpio_Setstate(NRF_MOSI_PIN,x)
#define NRF_SCK(x)  gpio_Setstate(NRF_SCK_PIN,x)

#define NRF_MISO    gpio_GetIO(NRF_MISO_PIN)
#define NRF_IRQ     gpio_GetIO(NRF_IRQ_PIN)
uint8_t NRF_RF_Addr[NRF_ADDR_WIDTH]={0xff,0xff,0xff,0xff,0xff};   // TX,RX��ַ,�����и���

uint8_t nrf_rx_buff[NRF_DATA_WIDTH];                      	// �������ݻ�����
uint8_t nrf_tx_buff[NRF_DATA_WIDTH];                      	// �������ݻ�����
// ����: nrf_spi_wr_byte()
// ����: ͨ��SPIдһ��byte��nRF24L01+,ͬʱ��nRF24L01+��ȡһ��byte
// ����: byte   ��д���ֽ�����
//       return ��ȡ�����ֽ�����
//		�ڲ�ʹ���û��������
uint8_t nrf_spi_wr_byte(uint8_t byte)
{
    uint8_t i;
		for(i=0; i<8; i++){                            		// ѭ��8��
				NRF_MOSI(byte & 0x80);               			// ��byte���λ�����NRF_MOSI_PIN
				byte <<= 1;                                		// ��һλ��λ�����λ
				NRF_SCK(1);                            		// ����NRF_SCK_PIN,nRF24L01��NRF_MOSI_PIN����1λ����,ͬʱ��NRF_MISO_PIN���1λ����
				byte |= NRF_MISO;                       		// ��NRF_MISO_PIN��byte���λ
				NRF_SCK(0);                            		// NRF_SCK_PIN�õ�
		}	
    return(byte);                                      		// ���ض�����һ�ֽ�
}
// ����: nrf_spi_w_cmd_byte()
// ����: ��������д��nRF24L01+,ͬʱ����status�ֽ�
// ����: cmd    ������
//    return ����status״̬
//		�ڲ�ʹ���û��������
uint8_t nrf_spi_w_cmd_byte(uint8_t cmd)
{
    uint8_t status;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    status = nrf_spi_wr_byte(cmd);                      	// д��Ĵ�����ַ,ͬʱ����״̬��
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}
// ����: nrf_spi_w_reg_byte()
// ����: ��valд��addr��Ӧ�ļĴ�����ַ,ͬʱ����status�ֽ�
// ����: cmd    ������
//       val    ��д��Ĵ�������ֵ
//       return ����status״̬
//		�ڲ�ʹ���û��������
uint8_t nrf_spi_w_reg_byte(uint8_t cmd, uint8_t val)
{
    uint8_t status;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    status = nrf_spi_wr_byte(cmd);                      	// д��Ĵ�����ַ,ͬʱ����״̬��
    nrf_spi_wr_byte(val);                               	// Ȼ��д���ݵ��üĴ���
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}

// ����: nrf_spi_r_reg_byte()
// ����: ��ȡaddr����Ӧ�ļĴ�����ַ
// ����: cmd    ������
//       return ���ؼĴ����ڵ���ֵ
//		�ڲ�ʹ���û��������
uint8_t nrf_spi_r_reg_byte(uint8_t cmd)
{
    uint8_t val;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    nrf_spi_wr_byte(cmd);                               	// д��Ĵ�����ַ
    val = nrf_spi_wr_byte(0);                           	// Ȼ��ӸüĴ���������
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(val);                                        	// ���ؼĴ�������
}

// ����: nrf_spi_w_reg_burst()
// ����: ����д��len���ֽڵ�addrָ���ļĴ�����,ͬʱ����status״̬��
// ����: cmd    ������
//       *p     ��д������ָ��
//       len    ��д�����ݳ���
//       return ����status״̬
//		�ڲ�ʹ���û��������
uint8_t nrf_spi_w_reg_burst(uint8_t cmd, uint8_t *p, uint8_t len)
{
    uint8_t status, i;
    NRF_CSN (0);                                    		 // NRF_CSN_PIN�õ�,��ʼ��������
    status = nrf_spi_wr_byte(cmd);                      	// ѡ��Ĵ���,ͬʱ����״̬��
    for(i=0; i<len; i++)	
        nrf_spi_wr_byte(p[i]);                          	// ����ֽ�д��nRF24L01+
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}

// ����: nrf_spi_r_reg_burst()
// ����: ������ȡlen���ֽڵ�addrָ���ļĴ�����,ͬʱ����status״̬��
// ����: cmd     ������
//       *p     ����ȡ����ָ��
//       len    ����ȡ���ݳ���
//       return ����status״̬
//		�ڲ�ʹ���û��������
uint8_t nrf_spi_r_reg_burst(uint8_t cmd, uint8_t *p, uint8_t len)
{
    uint8_t status, i;
    NRF_CSN (0);                                    		// NRF_CSN_PIN�õ�,��ʼ��������
    status = nrf_spi_wr_byte(cmd);                      	// ѡ��Ĵ���,ͬʱ����״̬��
    for(i=0; i<len; i++)	
        p[i] = nrf_spi_wr_byte(0);                      	// ����ֽڴ�nRF24L01+����
    NRF_CSN (1);                                    		// NRF_CSN_PIN����,�������ݴ���
    return(status);                                     	// ����״̬�Ĵ���    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF��麯��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void nrf_self_check(void)
{
    uint8_t temp=0;

    while(NRF_DATA_WIDTH != temp)
	{
		nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_RX_PW_P0,   NRF_DATA_WIDTH);    // ����RXͨ��0���ݿ��
		temp = nrf_spi_r_reg_byte(NRF_CMD_R_REGISTER | NRF_REG_RX_PW_P0);               // ��ȡRXͨ��0���ݿ��
	}
    //�����������while����������¼������
    //1.nrfδ����
    //2.nrf�뵥Ƭ�����Ӳ��ɿ���������޷�ͨ�Ŷ�����Ϊ���
    //3.nrf�ǻ���,���NRF���µģ����Ŀ����Լ���
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF����ģ���ʼ��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void nrf_init(void)
{
    gpio_init(NRF_CE_PIN  ,3,0);
		gpio_Setstate(NRF_CE_PIN,0);
    gpio_init(NRF_CSN_PIN ,3,0);
		gpio_Setstate(NRF_CSN_PIN,0);
    gpio_init(NRF_MOSI_PIN,3,0);
		gpio_Setstate(NRF_MOSI_PIN,0);
    gpio_init(NRF_SCK_PIN ,3,0);
		gpio_Setstate(NRF_SCK_PIN,0);
              
    gpio_init(NRF_MISO_PIN,0,2);
    gpio_init(NRF_IRQ_PIN ,0,2);
		delay_ms(50);
    
    NRF_CE(0);                                    		// �������ģʽ
    NRF_CSN(1);                                    		// SPI��ֹ
    NRF_SCK(0);                                    		// SPIʱ���õ�
    
    NRF_CE(0);                                             // NRF_CE_PIN�õ�,�������ģʽ
    nrf_self_check();                                            // NRF���
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_CONFIG,     NRF_RCD_TX_DS_DI        // ���÷�������ж�
                                                              | NRF_RCD_MAX_RT_DI       // ���÷��ͳ�ʱ�ж�
                                                              | NRF_RCD_CRC_EN          // ����CRC
                                                              | NRF_RCD_CRC16           // CRC16
                                                              | NRF_RCD_PWR_UP          // �ϵ�ģʽ
                                                              | NRF_RCD_PRIM_PRX);      // RXģʽ
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_EN_AA,      NRF_RCD_PIPE0_AA_EN);   // ʹ��ͨ��0�Զ�Ӧ��
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_EN_RXADDR,  NRF_RCD_PIPE0_RX_EN);   // ʹ�ܽ���ͨ��0
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_SETUP_AW,   NRF_RCD_AW_5byte);      // ����RX��ַ���
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_SETUP_RETR, NRF_RCD_ARC_10|NRF_RCD_ARD_250us);        // ʹ���Զ��ط�  ���ʱ��250US
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_RF_CH,      NRF_RF_CH);             // ������Ƶͨ��
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_RF_SETUP,   NRF_RCD_RF_DR_2Mbps     // ���ô�������Ϊ2Mbps
                                                              | NRF_RCD_RF_PWR_0dBm);   // ���书��Ϊ0dBm
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_RX_PW_P0,   NRF_DATA_WIDTH);        // ����RXͨ��0���ݿ��

    nrf_spi_w_reg_burst(NRF_CMD_W_REGISTER | NRF_REG_RX_ADDR_P0, NRF_RF_Addr, NRF_ADDR_WIDTH);   // ����RXͨ��0��ַ
    nrf_spi_w_cmd_byte(NRF_CMD_FLUSH_RX);                                               // ���RXFIFO
    NRF_CE(1);                                                                     	// NRF_CE_PIN�ø�,��������ģʽ
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF����ģ�����ݷ��ͺ���
//  @param      *p			Ҫ���͵����ݵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:			��ʼ��nRF24L01+,������ת��Ϊ����״̬���������ݻ������е�����,������ɺ���ת��Ϊ����״̬
//-------------------------------------------------------------------------------------------------------------------
void nrf_send_packet(uint8_t *p)
{
    uint8_t sta=0;
    
    NRF_CE(0);                                                                     	// NRF_CE_PIN�õ�,�������ģʽ
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_CONFIG, NRF_RCD_TX_DS_DI            // ���÷�������ж�
                                                          | NRF_RCD_MAX_RT_DI           // ���÷��ͳ�ʱ�ж�
                                                          | NRF_RCD_CRC_EN              // ����CRC
                                                          | NRF_RCD_CRC16               // CRC16
                                                          | NRF_RCD_PWR_UP              // �ϵ�ģʽ
                                                          | NRF_RCD_PRIM_PTX);          // TXģʽ

    nrf_spi_w_reg_burst(NRF_CMD_W_REGISTER | NRF_REG_TX_ADDR, NRF_RF_Addr, NRF_ADDR_WIDTH);  // ����TX��ַ
    nrf_spi_w_reg_burst(NRF_CMD_W_TX_PAYLOAD, p, NRF_DATA_WIDTH);                       // д�뷢������
    NRF_CE(1);                                                                     	// NRF_CE_PIN�ø�,��������ģʽ

    while(1)
    {
        sta = nrf_spi_r_reg_byte(NRF_CMD_R_REGISTER | NRF_REG_STATUS);
        if(sta&0x10)//����ʧ��
        {
            nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_STATUS, 0x10); 				// д��TX_DR,����NRF_IRQ_PIN�ж�
            nrf_spi_w_reg_byte(NRF_CMD_FLUSH_TX,0xff);
            break;
        }
        if(sta&0x20)//���ͳɹ�
        {
            nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_STATUS, NRF_RCD_TX_DR); 	// д��TX_DR,����NRF_IRQ_PIN�ж�
            break;   
        }
    }
    NRF_CE(0);                                                                     	// NRF_CE_PIN�õ�,�������ģʽ
    nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_CONFIG,     NRF_RCD_TX_DS_DI        // ���÷�������ж�
                                                              | NRF_RCD_MAX_RT_DI       // ���÷��ͳ�ʱ�ж�
                                                              | NRF_RCD_CRC_EN          // ����CRC
                                                              | NRF_RCD_CRC16           // CRC16
                                                              | NRF_RCD_PWR_UP          // �ϵ�ģʽ
                                                              | NRF_RCD_PRIM_PRX);      // RXģʽ
    NRF_CE(1);                                                                     	// NRF_CE_PIN�ø�,��������ģʽ
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      NRF����ģ�����ݽ��պ���
//  @param      *p			׼�����յ����ݵ������ַ
//  @return     uint8		���ճɹ���־,Ϊ1ʱ�������ݽ��ճɹ�
//  @since      v1.0		
//  Sample usage:			���nRF24L01+�Ƿ���յ�����,���յ�����ʱ�����ݶ�ȡ���������ݻ�������
//-------------------------------------------------------------------------------------------------------------------
uint8_t nrf_rece_packet(uint8_t *p)
{
    uint8_t sta=0;
    if(NRF_IRQ==0){                                                                 	// �ȴ�NRF_IRQ_PIN�õ�
        sta = nrf_spi_r_reg_byte(NRF_CMD_R_REGISTER | NRF_REG_STATUS);                  // ��ȡSTATUS�Ĵ���
        if(sta & NRF_RCD_RX_DR){                                                        // RX_DSΪ1ʱ�������յ�����
            nrf_spi_w_reg_byte(NRF_CMD_W_REGISTER | NRF_REG_STATUS, NRF_RCD_RX_DR);     // д��RX_DS,����NRF_IRQ_PIN�ж�
            nrf_spi_r_reg_burst(NRF_CMD_R_RX_PAYLOAD, p, NRF_DATA_WIDTH);               // ��ȡnRF24L01+�еĽ�������
            return(1);                                                                  // ���ؽ�����ɱ�־
        }
    }
    return(0);                                                                          // ���ؽ��ռ��ʧ�ܱ�־
}
//��д����,������޹�//
void clean_buff(uint8_t *p)
{
	while(*p!=0)
	{
		*p=0;
		p++;
	}
}
//���Գ���//
void nrf_test_sent()
{
	nrf_init();
	gpio_init(A3,3,0);
	gpio_init(A4,3,0);
	gpio_init(A5,3,0);
	gpio_init(A6,3,0);
	while(1)
	{
		gpio_SetIO(A3);
		gpio_SetIO(A4);
		gpio_SetIO(A5);
		gpio_SetIO(A6);
		nrf_send_packet("123");//����
		delay_ms(500);
	}
}
void nrf_text_get()
{
	nrf_init();
	gpio_init(A3,3,0);
	gpio_init(A4,3,0);
	gpio_init(A5,3,0);
	gpio_init(A6,3,0);
	while(1)
	{
		gpio_SetIO(A3);
		gpio_SetIO(A4);
		gpio_SetIO(A5);
		gpio_SetIO(A6);
		clean_buff(nrf_rx_buff);//����
		nrf_rece_packet(nrf_rx_buff);//����
		//USART_Sent_String(USART1,nrf_rx_buff,1);//����(��Ҫ��ʼ������)
		delay_ms(500);
	}
}



