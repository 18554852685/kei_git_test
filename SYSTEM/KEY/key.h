#ifndef __KEY_H
#define __KEY_H	 


#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键3(WK_UP) 

#define KEY4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)//读取按键0
#define KEY5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)//读取按键1
#define KEY6  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)//读取按键2 
#define KEY7  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)//读取按键3(WK_UP) 


void KEY_Init(void);//IO初始化
unsigned char  KEY_Scan1(void);  	//按键扫描函数			
unsigned char  KEY_Scan2(void);  	//按键扫描函数		
#endif
