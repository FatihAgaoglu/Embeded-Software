#include "stm32f10x.h"                  // Device header

//uint16_t buttonstate=0;
uint16_t flag=0;

void gpioConfig(){
	
//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC ->APB2ENR = 0X00000018;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIOB ->CRL = 0X00000333;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIOC ->CRL = 0X08000000;
	
//	GPIO_Init(GPIOC,&GPIOInitStructure);
}

void delay(uint32_t time){
	
	while(time--);
}

int main(){
	
	gpioConfig();
	
while(1){
	
//	buttonstate=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
	
	while(!(GPIOC ->IDR & 0X00000040));
	
	if(GPIOC ->IDR & 0X00000040){
		if(flag==0){
			flag=1;
		//	GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			GPIOB ->BSRR = 0X00000007;
}
		else{
			flag=0;
		//	GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			GPIOB ->BSRR = 0X00070000;
			}
		delay(7200000);
		}
	}
}
