#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

void gpioConfig(){
	
	RCC ->APB2ENR = 0X00000019; 		// adjust AFIO settings
	
	GPIOB ->CRL = 0X00000333;
	GPIOC ->CRL = 0X08000000;
	
}

void extiConfig(){
	
	AFIO ->EXTICR[1] = 0X00000200;

	
	EXTI ->IMR = 0X00000040;
	EXTI ->EMR = 0X00000000;
	EXTI ->FTSR = 0X00000000;
	EXTI ->RTSR = 0X00000040;
	EXTI ->SWIER = 0X00000000;
	
	NVIC ->IP[23] = 0X30;
	NVIC_EnableIRQ(23); 			//NVIC ->ISER[23>>30]=1<< (23 & 0X1F);

	
}

void delay (uint32_t time){
	
	while (time--);
	
}

void EXTI9_5_IRQHandler()
{
	
	if(EXTI->PR & 0X00000040)
	{
		for(int i=0; i<4; i++)
		{
			GPIOB ->BSRR = 0X00000007;
			delay(3600000);
			GPIOB ->BSRR = 0X00070000;
			delay(3600000);
		}
	EXTI ->PR = 0X00000040;
	}
}

int main(){
	
	gpioConfig();
	extiConfig();
	
	while(1){
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
