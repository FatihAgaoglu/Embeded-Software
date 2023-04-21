#include "stm32f10x.h"                  // Device header

uint16_t flag=0;

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
	
	
	while(!(GPIOC ->IDR & 0X00000040));
	
	if(GPIOC ->IDR & 0X00000040){
		if(flag==0){
			flag=1;
			GPIOB ->BSRR = 0X00000007;
}
		else{
			flag=0;
			GPIOB ->BSRR = 0X00070000;
			}
		delay(7200000);
		}
	}
}
