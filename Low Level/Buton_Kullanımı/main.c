#include "stm32f10x.h"                  // Device header


void gpioConfig(){
	
	RCC ->APB2ENR = 0X00000018;
	
	GPIOB ->CRL = 0X00000003;
	
	GPIOC ->CRL = 0X08000000;
	
}

int main(){
	
	gpioConfig();
	
while(1){
	
	
	if((GPIOC ->IDR & 0X00000040)){
		
		GPIOB ->BSRR = 0X00000001;
	}
	else{
		
		GPIOB ->BSRR = 0X00010000;
		}
	}
}
