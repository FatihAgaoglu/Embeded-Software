#include "stm32f10x.h"                  // Device header


void gpioConfig(){

//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC ->APB2ENR = 0X00000008;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOB ->CRL = 0X00000003;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);

}

int main() {
	gpioConfig();

while(1) {

	for(int i=0; i<36000; i++);
	GPIOB ->BSRR = 0X00000001;
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	for(int i=0; i<360000; i++);
	GPIOB ->BSRR = 0X00010000;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	for(int i=0; i<36000; i++);
	GPIOB ->BSRR = 0X00000001;
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	for(int i=0; i<720000; i++);
	GPIOB ->BSRR = 0X00010000;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	for(int i=0; i<36000; i++);
	GPIOB ->BSRR = 0X00000001;
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	for(int i=0; i<1080000; i++);
	GPIOB ->BSRR = 0X00010000;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	for(int i=0; i<36000; i++);
	GPIOB ->BSRR = 0X00000001;
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	for(int i=0; i<2160000; i++);
	GPIOB ->BSRR = 0X00010000;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
	}
}
