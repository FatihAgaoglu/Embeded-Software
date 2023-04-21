#include "stm32f10x.h"                  // Device header

void gpioConfig(){
	
	RCC ->APB2ENR = 0X00000008;

	GPIOB ->CRL = 0X00000003;


}

void delay(uint32_t time){

	while(time--);
}

int main(){

	gpioConfig();
	
	while(1){
		GPIOB ->BSRR = 0X00000001;
		delay(360000);
		GPIOB ->BSRR = 0X00010000;
		delay(360000);
	}
}
	
	
	