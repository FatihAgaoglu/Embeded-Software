#include "stm32f10x.h"                  // Device header

uint16_t buttonstate=0;

void gpioConfig(){
	
//	GPIO_InitTypeDef GPIOInitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC ->APB2ENR = 0X00000018;
//	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIOB ->CRL = 0X00000333;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);
//	
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
//	int LedArray[3]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2};
	while(1){
//		buttonstate=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
		
		if(GPIOC ->IDR & 0X00000040){
			
	//		for(int i=0; i<3 ; i++){
	//		GPIO_SetBits(GPIOB,LedArray[i]);
				GPIOB ->BSRR = 0X00060001;
			delay(360000);
				GPIOB ->BSRR = 0X00050002;
			delay(360000);
				GPIOB ->BSRR = 0X00030004; 
	//		GPIO_ResetBits(GPIOB,LedArray[i]);
			delay(360000);
				GPIOB ->BSRR = 0X00050002;
	//	}
	//	for(int i=1; i>0 ; i--){
	//		GPIO_SetBits(GPIOB,LedArray[i]);
			delay(360000);
	//		GPIO_ResetBits(GPIOB,LedArray[i]);
	//		delay(360000);	
	//	}			
	}
		else{
	//		for(int i=0; i<3 ; i++){
	//		GPIO_SetBits(GPIOB,LedArray[i]);
				GPIOB ->BSRR = 0X00060001;
			delay(3600000);
	//		GPIO_ResetBits(GPIOB,LedArray[i]);
				GPIOB ->BSRR = 0X00050002;
			delay(3600000);
				GPIOB ->BSRR = 0X00030004; 
	//	}
	//	for(int i=1; i>0 ; i--){
	//		GPIO_SetBits(GPIOB,LedArray[i]);
			delay(3600000);
				GPIOB ->BSRR = 0X00050002;
	//		GPIO_ResetBits(GPIOB,LedArray[i]);
			delay(3600000);	
	//		}
		}
	}
}
