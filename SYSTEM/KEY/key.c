#include "stm32f10x.h"
#include "key.h"
#include "delay.h"

								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʼ��KEY0-->GPIOA.13,KEY1-->GPIOA.15  ��������
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;//PC6_13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC6_13
}

//void delay_init(unsigned char SYSCLK)
//{
//	
//}

u8 KEY_Scan1(void)
{
	u8 keyID1;
	
	u8 KEY1_1=0x00;
	u8 KEY2_1=0x00;
	u8 KEY3_1=0x00;
	u8 KEY4_1=0x00;

	if(KEY0!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY0!=1)
		{
			KEY1_1=0x01;
		}
	}
	if(KEY1!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY1!=1)
		{
			KEY2_1=0x02;
		}
	}
  if(KEY2!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY2!=1)
		{
			KEY3_1=0x04;
		}
	}
	if(KEY3!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY3!=1)
		{
			KEY4_1=0x08;
		}
	}
	
	keyID1=0x00|KEY1_1|KEY2_1|KEY3_1|KEY4_1;
	return keyID1;
}

u8 KEY_Scan2(void)
{
	u8 keyID2;
	
	u8 KEY5_1=0x00;
	u8 KEY6_1=0x00;
	u8 KEY7_1=0x00;
	u8 KEY8_1=0x00;
	if(KEY4!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY4!=1)
		{
			KEY5_1=0x01;
		}
	}
	if(KEY5!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY5!=1)
		{
			KEY6_1=0x02;
		}
	}
	if(KEY6!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY6!=1)
		{
			KEY7_1=0x04;
		}
	}
	if(KEY7!=1)
	{
		delay_ms(10);//ȥ���� 
		if(KEY7!=1)
		{
			KEY8_1=0x08;
		}
	}
	keyID2=0x00|KEY5_1|KEY6_1|KEY7_1|KEY8_1;
	return keyID2;
}
