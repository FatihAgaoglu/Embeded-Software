#include "stm32f10x.h"                  // Device header

uint16_t counter=0;

void gpioConfig(){
	

		RCC ->APB2ENR = 0X00000018;

		GPIOB ->CRL = 0X00000003;

		GPIOC ->CRL = 0X08000000;

}

void delay(uint32_t time){
	
	while(time--);
}

int main(){
	
	gpioConfig();
	
while(1){
	
	
	if((GPIOC ->IDR & 0X00000040)){
		
		counter++;
		delay(36000);
	}
	
	if(GPIOC ->IDR & 0X00000040){
		
		GPIOB ->BSRR = 0X00000001;
		delay(360000);
	}
	
	else{
		
		GPIOB ->BSRR = 0X00010000;
		delay(360000);
		
		}
	}
}
