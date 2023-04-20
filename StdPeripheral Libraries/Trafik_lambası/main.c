#include "stm32f10x.h"                  // Device header

void gpioConfig(){

// GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC ->APB2ENR = 0X00000008;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOB ->CRL = 0X00000333;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);

}

void delay(uint32_t time){
	
	while(time--);
	
}

int main(){

	gpioConfig();
	
while(1){

//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	delay(72000000);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	GPIO_SetBits(GPIOB,GPIO_Pin_1);
//	delay(3600000);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
//	GPIO_SetBits(GPIOB,GPIO_Pin_2);
//	delay(72000000);
//	GPIO_SetBits(GPIOB,GPIO_Pin_1);
//	delay(3600000);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_2);

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
