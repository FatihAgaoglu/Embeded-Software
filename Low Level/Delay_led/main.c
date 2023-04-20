#include "stm32f10x.h"                  // Device header

void gpioConfig(){

//	GPIO_InitTypeDef	GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC ->APB2ENR = 0X00000008;

//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOB ->CRL = 0X00000003;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);

}

void delay(uint32_t time){

	while(time--);
}

int main(){

	gpioConfig();
	
	while(1){
//		GPIO_SetBits(GPIOB,GPIO_Pin_0);
//		delay(3600000);
//		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//		delay(3600000);
//		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
		GPIOB ->BSRR = 0X00000001;
		delay(360000);
		GPIOB ->BSRR = 0X00010000;
//		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
		delay(360000);
	}
}
	
	
	