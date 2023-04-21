#include "stm32f10x.h"                  // Device header

void gpioConfig(){
	RCC ->APB2ENR = 0X00000008;
	
	GPIOB ->CRL = 0X00000333;

}

void delay(uint32_t time){
	
	while(time--);
	
}

int main(){

	gpioConfig();
	
while(1){

	GPIOB ->BSRR = 0X00060001;
	delay(72000000);
	GPIOB ->BSRR = 0X00050002;
	delay(3600000);
	GPIOB ->BSRR = 0X00030004;
	delay(72000000);
	GPIOB ->BSRR = 0X0010006;
	delay(3600000);
	
	}
}
