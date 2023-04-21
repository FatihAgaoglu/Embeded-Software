#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

uint16_t counter=0;

void gpioConfig(){
	
	RCC ->APB2ENR = 0X00000019;
	
	GPIOB ->CRL = 0X00000333;
	GPIOC ->CRL = 0X08000000;
	
	
}

void delay (uint32_t time){
	
	while (time--);
	
}

void extiConfig(){
	
	AFIO ->EXTICR[1] = 0X00000200;
	
	EXTI ->IMR = 0X00000040;
	EXTI ->RTSR = 0X00000040;
	
	NVIC ->IP[23] = 0X30;
	NVIC_EnableIRQ(23); 			//NVIC ->ISER[23>>30]=1<< (23 & 0X1F);
	
	
}

void EXTI9_5_IRQHandler(){
	
	if(EXTI->PR & 0X00000040)
	{
		counter++;
		GPIOB ->BSRR = 0X00070000;
		delay(3600000);
	}
	EXTI ->PR = 0X00000040;
}

int main(){
	gpioConfig();
	extiConfig();
	
	while(1)
	{
		GPIOB ->BSRR = 0X00000007;
		delay(360000);
		GPIOB ->BSRR = 0X00070000;
		delay(360000);
	}	
}
