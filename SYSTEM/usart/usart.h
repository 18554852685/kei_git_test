#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

extern unsigned char modbusrecv[8];
extern unsigned char receiveCount;
extern unsigned char modbusID;
extern unsigned char registerID;

extern float temp0;
extern float temp1;
extern float temp2;
extern float temp3;
extern float temp4;
extern float temp5;
extern float temp6;
extern float temp7;
extern float temp8;
extern float temp9;
extern float temp10;
extern float temp11;
extern float temp12;
extern float temp13;
extern float temp14;
extern float temp15;

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
unsigned int crc16r(unsigned char* puchMsg,unsigned char usDataLen);
//void modbusSentBack(unsigned char *modbusSent);
void get_NTC20K_Temperature_Caculate(float TemperatureTempx);
void get_NTC20K_Temperature(unsigned char hex);
void get_DI_Caculate(float DItempx);
void get_DI(unsigned char hex);
void get_V_Caculate(float Vtempx);
void get_V(unsigned char hex);
void get_mA_Caculate(float mAtempx);
void get_mA(unsigned char hex);
#endif


