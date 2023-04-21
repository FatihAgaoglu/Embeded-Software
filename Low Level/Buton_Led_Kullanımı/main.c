#include "stm32f10x.h"                  // Device header

uint16_t counter=0;

void gpioConfig(){
	
		RCC ->APB2ENR = 0X00000018;
	
		GPIOB ->CRL = 0X00000333;
	
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
		delay(3600000);
		
	if(counter%3==1){
		GPIOB ->BSRR = 0X00060001;
	}
	else if(counter%3==2){
		GPIOB ->BSRR = 0X00050002;
	}
	else if(counter%3==0){
		GPIOB ->BSRR = 0X00000007;
	}
	else {
		GPIOB ->BSRR = 0X00070000;
			}
		}
	}
}
