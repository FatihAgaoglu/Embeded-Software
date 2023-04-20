#include "stm32f10x.h"                  // Device header


void gpioConfig(){

//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC ->APB2ENR = 0X00000008;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOB ->CRL = 0X00000003;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
//GPIO_Init(GPIOB,&GPIOInitStructure);
}


int main(){
	gpioConfig();
	
	while(1){
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIOB ->BSRR = 0X00000001;
	
	}
}
