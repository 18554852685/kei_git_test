#include "stm32f10x.h"
#include "delay.h"

static unsigned char fac_us=0;
static unsigned short fac_ms=0;

//void delay_init (unsigned char SYSCLK)
//{
//	SysTick->CTRL&=0xfffffffb; //bit2 清空，选择外部时钟 HCLK/8
//	fac_us=SYSCLK/8;
//	fac_ms=(unsigned short)fac_us*1000;
//}

void delay_init (unsigned char SYSCLK)
{
	SysTick->CTRL&=0xfffffffb; //bit2 清空，选择外部时钟 HCLK/8
	fac_us=SYSCLK/8;
	fac_ms=(unsigned short)fac_us*1000;
}


void delay_ms(unsigned short nms)
{
	unsigned int temp;
	SysTick->LOAD=(unsigned int)nms*fac_ms;
	SysTick->VAL=0x00;
	SysTick->CTRL=0x01;
	do 
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;
	SysTick->VAL=0x00;
	
}




























