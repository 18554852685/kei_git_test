#include "mcp6s28spi.h"
#include "delay.h"
#include "stm32f10x_spi.h"

void SPI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE );//PORTBʱ��ʹ�� 
	/*!< SPI_FLASH_SPI Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/*!< AFIO Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                    			//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;                  //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                    			//
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOA,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;  // PB12 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
  GPIO_SetBits(GPIOC,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);

	SPI_Cmd(SPI1, DISABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	  //����ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							  //����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;						  //8λģʽ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							  //SCLK����ʱλ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;							  //SCLK���½��زɼ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								  //�ӻ�ѡ���źţ����ģʽ��������GPIOѡ��ӻ�
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	  //APB2����APB1����Ƶ�ʵ�1/2~1/256��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					  //���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;								  //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
}   


//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(unsigned char TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		
	}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}


void SPI1_MCP6S28_WriteREG(unsigned char regaddr,unsigned char databyte)
{
    SPI1_ReadWriteByte (regaddr);
    SPI1_ReadWriteByte(databyte);
}

void MCP6S28_Read_Config(unsigned char channel)
{
	if((channel>=1)&&(channel<=8))
	{
//		GPIO_SetBits(GPIOC,GPIO_Pin_1);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
//		
//		delay_ms(5);
//		SPI1_MCP6S28_WriteREG(0x41,8-channel);
//		delay_ms(5);
//		GPIO_SetBits(GPIOC,GPIO_Pin_2);
//		delay_ms(1);
//		GPIO_SetBits(GPIOC,GPIO_Pin_1);
//		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
//		
//		delay_ms(5);
////		SPI1_ReadWriteByte(0x00);
//		SPI1_MCP6S28_WriteREG(0x00,0);	
	}
	else if((channel>=9)&&(channel<=16))
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_2);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//		
		delay_ms(5);
		SPI1_MCP6S28_WriteREG(0x41,16-channel);
		delay_ms(5);
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
		delay_ms(1);
		GPIO_SetBits(GPIOC,GPIO_Pin_2);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//		
		delay_ms(5);
//		SPI1_ReadWriteByte(0x00);
		SPI1_MCP6S28_WriteREG(0x00,0);
	}
}

//void SPI1_MCP6S28_Init(void)
//{
//	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
//	delay_ms(10);
////	SPI1_ReadWriteByte(0x4003);
//	SPI1_MCP6S28_WriteREG(0x41,0x03);
//	delay_ms(10);
//	GPIO_SetBits(GPIOC,GPIO_Pin_1);
////	SPI1_MCP6S28_WriteREG(0x40,0x03);
//	
//	delay_ms(10);
//	
//}





























