#ifndef __KEY_H
#define __KEY_H	 


#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����3(WK_UP) 

#define KEY4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)//��ȡ����0
#define KEY5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//��ȡ����1
#define KEY6  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//��ȡ����2 
#define KEY7  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//��ȡ����3(WK_UP) 


void KEY_Init(void);//IO��ʼ��
unsigned char  KEY_Scan1(void);  	//����ɨ�躯��			
unsigned char  KEY_Scan2(void);  	//����ɨ�躯��		
#endif
