#ifndef __SPI_H
#define __SPI_H
//#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

 
 				  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
//void SPI2_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
unsigned char  SPI1_ReadWriteByte(unsigned char TxData);//SPI���߶�дһ���ֽ�

void SPI1_MCP6S28_WriteREG(unsigned char regaddr,unsigned char databyte);
void MCP6S28_Read_Config(unsigned char channel);
		 
#endif

