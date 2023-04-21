#include "stm32f10x.h"                  // Device header

uint16_t buttonstate=0;

void gpioConfig(){

	RCC ->APB2ENR = 0X00000018;

	GPIOB ->CRL = 0X00000333;
	GPIOC ->CRL = 0X08000000;
	
}

void delay(uint32_t time){
	
	while(time--);
}


int main()
{
	gpioConfig();
	while(1)
	{
		
		if(GPIOC ->IDR & 0X00000040)
		{
			GPIOB ->BSRR = 0X00060001;
			delay(360000);
			GPIOB ->BSRR = 0X00050002;
			delay(360000);
			GPIOB ->BSRR = 0X00030004; 
			delay(360000);
			GPIOB ->BSRR = 0X00050002;
			delay(360000);		
	}
		else
		{
			GPIOB ->BSRR = 0X00060001;
			delay(3600000);
			GPIOB ->BSRR = 0X00050002;
			delay(3600000);
			GPIOB ->BSRR = 0X00030004; 
			delay(3600000);
			GPIOB ->BSRR = 0X00050002;
			delay(3600000);	
		}
	}
}
