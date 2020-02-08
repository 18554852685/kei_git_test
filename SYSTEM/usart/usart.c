#include "sys.h"
#include "usart.h"	 
#include "delay.h"
#include "led.h"

//unsigned char modbusSent[40];
unsigned char usartReceiveBit=0;

#if 1
#pragma import(__use_no_semihosting)                            
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
int _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);  
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


 
#if EN_USART1_RX   //如果使能了接收

//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;       

//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	USART_DeInit(USART1);  //复位串口1
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

 //Usart1 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
 //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}


unsigned int crc16r(unsigned char* puchMsg,unsigned char usDataLen)
{
	/* Table Of CRC Values for high-order unsigned char */
	const unsigned char auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40
	};

	/* Table of CRC values for low-order unsigned char    */
	const unsigned char auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40
}; 
	unsigned char uchCRCHi = 0xFF ;
  unsigned char uchCRCLo = 0xFF ;
  unsigned int  uIndex  ;
  while (usDataLen--)
	 {
			uIndex = uchCRCHi ^ *puchMsg++;
			uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
			uchCRCLo = auchCRCLo[uIndex] ;
	 }
	return (((unsigned int)(uchCRCHi) << 8) | uchCRCLo) ;
}

//void modbusSentBack(unsigned char *modbusSent[])
//{
//	unsigned int a;
//	
//}

void get_NTC20K_Temperature_Caculate(float TemperatureTempx)
{
	unsigned short TXcrc16resultCalculate;
	unsigned int Temperaturelist=0;
	float RtX;
	float RtX_X;
	
	float NTCTable[]={
		 70.200,66.515,63.046,59.777,56.697,53.793,51.055,48.472,46.034,43.733,41.560,
		 39.500,37.553,35.714,33.975,32.331,30.775,29.303,27.909,26.590,25.340,
		 24.155,23.032,21.967,20.958,20.000,19.089,18.224,17.404,16.624,15.884,
		 15.180,14.511,13.875,13.270,12.695,12.148,11.627,11.131,10.659,10.210,
		 9.781,9.373,8.983,8.612,8.258,7.920,7.598,7.291,6.998,6.718,
		 6.450,6.195,5.951,5.718,5.495,5.282,5.078,4.883,4.696,4.518,
		 4.347,4.184,4.027,3.877,3.734,3.596,3.464,3.338,3.216,3.100,
		 2.989,2.882,2.779,2.681,2.587,2.496,2.409,2.325,2.245,2.168,
		 2.094,2.022,1.954,1.888,1.824,1.763,1.705,1.648,1.594,1.542,
		 1.491,1.443,1.396,1.351,1.308
	 };	
		
	 float TemTable[]={
		 0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,
		 11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0,20.0,
		 21.0,22.0,23.0,24.0,25.0,26.0,27.0,28.0,29.0,30.0,
		 31.0,32.0,33.0,34.0,35.0,36.0,37.0,38.0,39.0,40.0,
		 41.0,42.0,43.0,44.0,45.0,46.0,47.0,48.0,49.0,50.0,
		 51.0,52.0,53.0,54.0,55.0,56.0,57.0,58.0,59.0,60.0,	
		 61.0,62.0,63.0,64.0,65.0,66.0,67.0,68.0,69.0,70.0,	 
		 71.0,72.0,73.0,74.0,75.0,76.0,77.0,78.0,79.0,80.0,	 
		 81.0,82.0,83.0,84.0,85.0,86.0,87.0,88.0,89.0,90.0,
		 91.0,92.0,93.0,94.0,95.0};
	 unsigned char modbusSent[40];
	 unsigned int a;
	modbusSent[0]=modbusID;
	modbusSent[1]=0x03;
	modbusSent[2]=0x04;	
	modbusSent[3]=0x00;
	modbusSent[4]=0x00;
		 
		 
	RtX=(TemperatureTempx*33.0)/(4.93-TemperatureTempx);
	for(Temperaturelist=0;Temperaturelist<96;Temperaturelist++)   /////////存在问题是若找到了阻值区间后Temperaturelist不到96，仍然进行操作
	{			
		if((RtX>NTCTable[Temperaturelist+1])&&RtX<NTCTable[Temperaturelist])
		{
			RtX_X=TemTable[Temperaturelist]+((NTCTable[Temperaturelist]-RtX)/(NTCTable[Temperaturelist]-NTCTable[Temperaturelist+1]));
		}
	}
	modbusSent[5]=(((int)(RtX_X*100))&0xff00)>>8;
  modbusSent[6]=(((int)(RtX_X*100))&0x00ff);
 
  TXcrc16resultCalculate=crc16r(modbusSent,7);
  modbusSent[7]=(unsigned char)((TXcrc16resultCalculate&0xff00)>>8);
  modbusSent[8]=(unsigned char)(TXcrc16resultCalculate&0x00ff);
	
	delay_ms(5);
	for(a=0;a<9;a++)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, modbusSent[a]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}	
}

void get_NTC20K_Temperature(unsigned char hex)
{
	switch(hex)
	{
		case 0x14:
			get_NTC20K_Temperature_Caculate(temp8);
			break;
		case 0x15:
			get_NTC20K_Temperature_Caculate(temp9);
			break;
		case 0x16:
			get_NTC20K_Temperature_Caculate(temp10);
			break;
		case 0x17:
			get_NTC20K_Temperature_Caculate(temp11);
			break;
		case 0x18:
			get_NTC20K_Temperature_Caculate(temp12);
			break;
		case 0x19:
			get_NTC20K_Temperature_Caculate(temp13);
			break;
		case 0x1a:
			get_NTC20K_Temperature_Caculate(temp14);
			break;
		case 0x1b:
			get_NTC20K_Temperature_Caculate(temp15);
			break;
		case 0x1c:
			get_NTC20K_Temperature_Caculate(temp0);
			break;
		case 0x1d:
			get_NTC20K_Temperature_Caculate(temp1);
			break;
		case 0x1e:
			get_NTC20K_Temperature_Caculate(temp2);
			break;
		case 0x1f:
			get_NTC20K_Temperature_Caculate(temp3);
			break;
		case 0x20:
			get_NTC20K_Temperature_Caculate(temp4);
			break;
		case 0x21:
			get_NTC20K_Temperature_Caculate(temp5);
			break;
		case 0x22:
			get_NTC20K_Temperature_Caculate(temp6);
			break;
		case 0x23:
			get_NTC20K_Temperature_Caculate(temp7);
			break;
		
		default : break;
	}	
}
void get_V_Caculate(float Vtempx)
{
	unsigned char modbusSent[40];
	unsigned int a;
	unsigned short TXcrc16resultCalculateV;
	float vValue=0.00;
	
	modbusSent[0]=modbusID;
	modbusSent[1]=0x03;
	modbusSent[2]=0x04;	
	modbusSent[3]=0x00;
	modbusSent[4]=0x00;
	
	vValue=(Vtempx*3.12766);
	
	modbusSent[5]=(((int)(vValue*100.0))&0xff00)>>8;
  modbusSent[6]=(((int)(vValue*100.0))&0x00ff);
	
	TXcrc16resultCalculateV=crc16r(modbusSent,7);
	modbusSent[7]=(unsigned char)((TXcrc16resultCalculateV&0xff00)>>8);
	modbusSent[8]=(unsigned char)(TXcrc16resultCalculateV&0x00ff);
	
	delay_ms(5);
	for(a=0;a<9;a++)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, modbusSent[a]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
}
void get_V(unsigned char hex)
{
	switch(hex) 
	{
		case 0x24:
			get_V_Caculate(temp8);
			break;
		case 0x25:
			get_V_Caculate(temp9);
			break;
		case 0x26:
			get_V_Caculate(temp10);
			break;
		case 0x27:
			get_V_Caculate(temp11);
			break;
		case 0x28:
			get_V_Caculate(temp12);
			break;
		case 0x29:
			get_V_Caculate(temp13);
			break;
		case 0x2a:
			get_V_Caculate(temp14);
			break;
		case 0x2b:
			get_V_Caculate(temp15);
			break;
		case 0x2c:
			get_V_Caculate(temp0);
			break;
		case 0x2d:
			get_V_Caculate(temp1);
			break;
		case 0x2e:
			get_V_Caculate(temp2);
			break;
		case 0x2f:
			get_V_Caculate(temp3);
			break;
		case 0x30:
			get_V_Caculate(temp4);
			break;
		case 0x31:
			get_V_Caculate(temp5);
			break;
		case 0x32:
			get_V_Caculate(temp6);
			break;
		case 0x33:
			get_V_Caculate(temp7);
			break;
		default :break ;		
	}
}

void get_mA_Caculate(float mAtempx)
{
	unsigned short TXcrc16resultCalculatemA;
	
	float mAvalue;
	unsigned char modbusSent[40];
	unsigned int a;
	modbusSent[0]=modbusID;
	modbusSent[1]=0x03;
	modbusSent[2]=0x04;	
	modbusSent[3]=0x00;
	modbusSent[4]=0x00;
	
	mAvalue=(mAtempx*1000.0)/160;
	
	modbusSent[5]=(((int)(mAvalue*100))&0xff00)>>8;
  modbusSent[6]=(((int)(mAvalue*100))&0x00ff);
	
	TXcrc16resultCalculatemA=crc16r(modbusSent,7);
	modbusSent[7]=(unsigned char)((TXcrc16resultCalculatemA&0xff00)>>8);
	modbusSent[8]=(unsigned char)(TXcrc16resultCalculatemA&0x00ff);
	
	delay_ms(5);
	for(a=0;a<9;a++)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, modbusSent[a]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
}
void get_mA(unsigned char hex)
{
	switch(hex) 
	{
		case 0x34:
			get_mA_Caculate(temp8);
			break;
		case 0x35:
			get_mA_Caculate(temp9);
			break;
		case 0x36:
			get_mA_Caculate(temp10);
			break;
		case 0x37:
			get_mA_Caculate(temp11);
			break;
		case 0x38:
			get_mA_Caculate(temp12);
			break;
		case 0x39:
			get_mA_Caculate(temp13);
			break;
		case 0x3a:
			get_mA_Caculate(temp14);
			break;
		case 0x3b:
			get_mA_Caculate(temp15);
			break;
		case 0x3c:
			get_mA_Caculate(temp0);
			break;
		case 0x3d:
			get_mA_Caculate(temp1);
			break;
		case 0x3e:
			get_mA_Caculate(temp2);
			break;
		case 0x3f:
			get_mA_Caculate(temp3);
			break;
		case 0x40:
			get_mA_Caculate(temp4);
			break;
		case 0x41:
			get_mA_Caculate(temp5);
			break;
		case 0x42:
			get_mA_Caculate(temp6);
			break;
		case 0x43:
			get_mA_Caculate(temp7);
			break;
		default :break ;		
	}
}

 void get_DI_Caculate(float DItempx)
{
	unsigned short TXcrc16resultCalculateDI;
	unsigned char modbusSent[40];
	unsigned int a;
	modbusSent[0]=modbusID;
	modbusSent[1]=0x03;
	modbusSent[2]=0x04;	
	modbusSent[3]=0x00;
	modbusSent[4]=0x00;
	
	if(DItempx>=1.0)
	{
		modbusSent[5]=0xff;
		modbusSent[6]=0xff;
	}
	else
	{
		modbusSent[5]=0x00;
		modbusSent[6]=0x00;	
	}
	TXcrc16resultCalculateDI=crc16r(modbusSent,7);
	modbusSent[7]=(unsigned char)((TXcrc16resultCalculateDI&0xff00)>>8);
	modbusSent[8]=(unsigned char)(TXcrc16resultCalculateDI&0x00ff);
	
	delay_ms(5);
	for(a=0;a<9;a++)
	{
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, modbusSent[a]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}
}

void get_DI(unsigned char hex)
{
	
	switch(hex) 
	{
		case 0x44:
			get_DI_Caculate(temp8);
			break;
		case 0x45:
			get_DI_Caculate(temp9);
			break;
		case 0x46:
			get_DI_Caculate(temp10);
			break;
		case 0x47:
			get_DI_Caculate(temp11);
			break;
		case 0x48:
			get_DI_Caculate(temp12);
			break;
		case 0x49:
			get_DI_Caculate(temp13);
			break;
		case 0x4a:
			get_DI_Caculate(temp14);
			break;
		case 0x4b:
			get_DI_Caculate(temp15);
			break;
		case 0x4c:
			get_DI_Caculate(temp0);
			break;
		case 0x4d:
			get_DI_Caculate(temp1);
			break;
		case 0x4e:
			get_DI_Caculate(temp2);
			break;
		case 0x4f:
			get_DI_Caculate(temp3);
			break;
		case 0x50:
			get_DI_Caculate(temp4);
			break;
		case 0x51:
			get_DI_Caculate(temp5);
			break;
		case 0x52:
			get_DI_Caculate(temp6);
			break;
		case 0x53:
			get_DI_Caculate(temp7);
			break;
		default :break ;		
	}
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		modbusrecv[receiveCount]=Res;
		receiveCount++;
		if(receiveCount>7)
		{
			if(modbusrecv[0]!=modbusID)
			{
//				receiveCount=0;
//				usartReceiveBit=0;
	//			USART_DeInit(USART1);  //复位串口1
	//			USART_Cmd(USART1, ENABLE);                    //使能串口 
				return;
			}
			else
				{
				receiveCount=0;
				usartReceiveBit=0;
				if((modbusrecv[7]|(modbusrecv[6]<<8))!=crc16r(modbusrecv,6))
				{		
					return;
				}
				if(!((modbusrecv[1]==0x03)&&(modbusrecv[5]==0x02)))
				{		
					return;
				}
				registerID=modbusrecv[2]<<8|modbusrecv[3];
				switch(registerID)
				{
					case 0x14:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x15:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x16:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x17:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x18:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x19:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x1a:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x1b:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x1c:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x1d:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x1e:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x1f:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x20:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x21:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x22:
						get_NTC20K_Temperature(registerID);
						break ;
					case 0x23:
						get_NTC20K_Temperature(registerID);
						break ;
					
					case 0x24:
						get_V(registerID);
						break ;
					case 0x25:
						get_V(registerID);
						break ;
					case 0x26:
						get_V(registerID);
						break ;
					case 0x27:
						get_V(registerID);
						break ;
					case 0x28:
						get_V(registerID);
						break ;
					case 0x29:
						get_V(registerID);
						break ;
					case 0x2a:
						get_V(registerID);
						break ;
					case 0x2b:
						get_V(registerID);
						break ;
					case 0x2c:
						get_V(registerID);
						break ;
					case 0x2d:
						get_V(registerID);
						break ;
					case 0x2e:
						get_V(registerID);
						break ;
					case 0x2f:
						get_V(registerID);
						break ;
					case 0x30:
						get_V(registerID);
						break ;
					case 0x31:
						get_V(registerID);
						break ;
					case 0x32:
						get_V(registerID);
						break ;
					case 0x33:
						get_V(registerID);
						break ;
					
					case 0x34:
						get_mA(registerID);
						break ;
					case 0x35:
						get_mA(registerID);
						break ;
					case 0x36:
						get_mA(registerID);
						break ;
					case 0x37:
						get_mA(registerID);
						break ;
					case 0x38:
						get_mA(registerID);
						break ;
					case 0x39:
						get_mA(registerID);
						break ;
					case 0x3a:
						get_mA(registerID);
						break ;
					case 0x3b:
						get_mA(registerID);
						break ;
					case 0x3c:
						get_mA(registerID);
						break ;
					case 0x3d:
						get_mA(registerID);
						break ;
					case 0x3e:
						get_mA(registerID);
						break ;
					case 0x3f:
						get_mA(registerID);
						break ;
					case 0x40:
						get_mA(registerID);
						break ;
					case 0x41:
						get_mA(registerID);
						break ;
					case 0x42:
						get_mA(registerID);
						break ;
					case 0x43:
						get_mA(registerID);
						break ;
	//				
					case 0x44:
						get_DI(registerID);
						break ;
					case 0x45:
						get_DI(registerID);
						break ;
					case 0x46:
						get_DI(registerID);
						break ;
					case 0x47:
						get_DI(registerID);
						break ;
					case 0x48:
						get_DI(registerID);
						break ;
					case 0x49:
						get_DI(registerID);
						break ;
					case 0x4a:
						get_DI(registerID);
						break ;
					case 0x4b:
						get_DI(registerID);
						break ;
					case 0x4c:
						get_DI(registerID);
						break ;
					case 0x4d:
						get_DI(registerID);
						break ;
					case 0x4e:
						get_DI(registerID);
						break ;
					case 0x4f:
						get_DI(registerID);
						break ;
					case 0x50:
						get_DI(registerID);
						break ;
					case 0x51:
						get_DI(registerID);
						break ;
					case 0x52:
						get_DI(registerID);
						break ;
					case 0x53:
						get_DI(registerID);
						break ;
					
					default :
						break;
	//				USART_ClearFlag(USART1,USART_FLAG_ORE);	// read SR
	//				USART_ReceiveData(USART1);				//read DR
				}
			}	
		 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}		
	}
	else if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_ORE);	// read SR
		USART_ReceiveData(USART1);				//read DR
	}
	else
	{
		LED0=0;
	}
}
#endif	

