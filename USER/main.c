#include <stdio.h>
#include "stm32f10x.h"
#include "mb.h"
#include "adc.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "STM32F10x_ADC.h"
#include "mcp6s28spi.h"



/* add test */
/* holding register address */
#define REG_HOLDING_START 0x00
/* number of regs */
#define REG_HOLDING_NREGS 128
/* content of the holding regs 
   the test data is from xukai's port */
//uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0x147b,0x3f8e,
//0x147b,0x400e,0x1eb8,0x4055,0x147b,0x408e};

signed short usRegHoldingBuf[REG_HOLDING_NREGS];


unsigned char LED_state=0; //LED0 PD2 High_Low_state 

/*����˼Ĭ��NTC20K_ B25-50��=4200K��ֵ��Ӧ��
���·�Χ��-10-95��
*/

		 
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
	
		/* Enable the TIM2 gloabal Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}
void Relay_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure; 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_0);						 //PB.5 �����   GPIO_SetBits
 GPIO_ResetBits(GPIOB,GPIO_Pin_10);						 //PB.5 �����   GPIO_ResetBits
 GPIO_ResetBits(GPIOB,GPIO_Pin_11);						 //PB.5 �����
 GPIO_ResetBits(GPIOB,GPIO_Pin_12);						 //PB.5 �����
 GPIO_ResetBits(GPIOB,GPIO_Pin_13);						 //PB.5 �����
 GPIO_ResetBits(GPIOB,GPIO_Pin_14);						 //PB.5 �����
 GPIO_ResetBits(GPIOB,GPIO_Pin_15);						 //PB.5 �����
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PB.5 �˿�����
 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOD,GPIO_Pin_2);						 //PB.5 �����   GPIO_SetBits

}
void shutdownALLRelay(void)                	//�ر����м̵���
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);	
//	GPIO_ResetBits(GPIOB,GPIO_Pin_13);	
	

}

int main(void)
{
	unsigned char key1=0;
	unsigned char key2=0;
	
	unsigned char SlaveID=0x01;
	
	unsigned int LEDLoop=0;
	
	
	
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_Configuration();
	
	delay_init(72);
	KEY_Init();
//	
//	
	LED_Init();
//	
	Relay_init();
	
	key1=KEY_Scan1();		//�õ���ֵ	
	key2=KEY_Scan2();		//�õ���ֵ		
	SlaveID=(key2*10)+key1;	
	SlaveID=1;


	eMBInit( MB_RTU, SlaveID, 0, 9600, MB_PAR_NONE );	
	/* Enable the Modbus Protocol Stack. */
	eMBEnable(  );
	
	for( ;; )
	{
		switch(usRegHoldingBuf[0])
		{
			case 0:
				shutdownALLRelay();
				break;
			
			case 5:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_11);	//��ʪ����
			  GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
//				GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
				break;
			
			case 9:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_12);	//��ʪ����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
				break;
			
			case 17:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_13);	//��ʪ����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15);
				break;
			
			case 32:
				GPIO_SetBits(GPIOB,GPIO_Pin_14);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);
				break;
			
			case 64:
				GPIO_SetBits(GPIOB,GPIO_Pin_15);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
				break;
			
			case 37:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_11);	//��ʪ����
				GPIO_SetBits(GPIOB,GPIO_Pin_14);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);
				break;
			
			case 69:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_11);	//��ʪ����
				GPIO_SetBits(GPIOB,GPIO_Pin_15);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
				break;
			
			case 41:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_12);	//��ʪ����
				GPIO_SetBits(GPIOB,GPIO_Pin_14);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15);
				break;
			
			case 73:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_12);	//��ʪ����
				GPIO_SetBits(GPIOB,GPIO_Pin_15);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14);
				break;
			
			case 49:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_13);	//��ʪ����
				GPIO_SetBits(GPIOB,GPIO_Pin_14);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15);
				break;
			
			case 81:
				GPIO_SetBits(GPIOB,GPIO_Pin_0);	//��ʪģʽ
				delay_ms(200);
				GPIO_SetBits(GPIOB,GPIO_Pin_13);	//��ʪ����
				GPIO_SetBits(GPIOB,GPIO_Pin_15);	//�·����
			GPIO_ResetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14);
				break;
			
			default:
				shutdownALLRelay();	
				break;
			
		}
		LEDLoop++;
		if((usRegHoldingBuf[0]!=32)&&(usRegHoldingBuf[0]!=64)&&(usRegHoldingBuf[0]!=0))
		{
			if(LEDLoop>=3)
			{
				LEDLoop=0;
				if(LED_state==0)  //ÿѭ������16��ADCͨ����ֵ��LED״̬ȡ��һ��;
				{
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
					LED_state=1;
				}
				else if(LED_state==1)
				{
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
					LED_state=0;
				}
				else
				{
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				
			}
		}
		else
		{
			if(LEDLoop>=65536)
			{
				LEDLoop=0;
				if(LED_state==0)  //ÿѭ������16��ADCͨ����ֵ��LED״̬ȡ��һ��;
				{
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
					LED_state=1;
				}
				else if(LED_state==1)
				{
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
					LED_state=0;
				}
				else
				{
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				
			}
		}
	  ( void )eMBPoll(  );
	}
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
	static uint8_t flag = 0;
	
	flag == 0 ? GPIO_SetBits(GPIOC,GPIO_Pin_1) : GPIO_ResetBits(GPIOC,GPIO_Pin_1);  
	flag ^= 1;
    return eStatus;
}



eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    /* error state */
    eMBErrorCode    eStatus = MB_ENOERR;
    /* offset */
    int16_t iRegIndex;
    
    /* test if the reg is in the range */
    if (((int16_t)usAddress-1 >= REG_HOLDING_START) 
        && (usAddress-1 + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        /* compute the reg's offset */
        iRegIndex = (int16_t)(usAddress-1 - REG_HOLDING_START);
			//
        switch (eMode)
        {
            case MB_REG_READ:
							
						while (usNRegs > 0)
                {
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingBuf[iRegIndex] >> 8 );
                    *pucRegBuffer++ = (uint8_t)( usRegHoldingBuf[iRegIndex] & 0xff);
                    iRegIndex ++;
                    usNRegs --;
                }
                break;
            case MB_REG_WRITE:
                while (usNRegs > 0)
                {
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    iRegIndex ++;
                    usNRegs --;
                }
                break;        
        }
    }
    else{
        eStatus = MB_ENOREG;
    }
    
    return eStatus;
    
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    static uint8_t flag = 0;
	
	flag == 0 ? GPIO_SetBits(GPIOC,GPIO_Pin_3) : GPIO_ResetBits(GPIOC,GPIO_Pin_3);  
	flag ^= 1;
	return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    static uint8_t flag = 0;
	
	flag == 0 ? GPIO_SetBits(GPIOC,GPIO_Pin_4) : GPIO_ResetBits(GPIOC,GPIO_Pin_4);  
	flag ^= 1;
	return MB_ENOREG;
}



