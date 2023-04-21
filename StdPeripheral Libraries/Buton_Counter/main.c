#include "stm32f10x.h"                  // Device header

uint16_t buttonstate=0;
uint16_t counter=0;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	
	GPIO_Init(GPIOC,&GPIOInitStructure);
}

void delay(uint32_t time){
	
	while(time--);
}

int main(){
	
	gpioConfig();
	
while(1){
	
	buttonstate=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
	
	if((GPIOC ->IDR & 0X00000040)){
		
		counter++;
		delay(36000);
	}
	
	if(GPIOC ->IDR & 0X00000040){
		
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		delay(360000);
	}
	
	else{
		
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		delay(360000);
		
		}
	}
}
