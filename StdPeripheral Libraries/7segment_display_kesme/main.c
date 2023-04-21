#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "seg7display.h"				// 7segment display library

uint16_t buttonstate=0;

void gpioConfig(){
	
	GPIO_InitTypeDef	GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	
	GPIO_Init(GPIOC,&GPIOInitStructure);
	
}

void extiConfig(){
	
	EXTI_InitTypeDef EXTIInitStructure;
	NVIC_InitTypeDef NVICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
	
	EXTIInitStructure.EXTI_Line=EXTI_Line6;
	EXTIInitStructure.EXTI_LineCmd=ENABLE;
	EXTIInitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	
	EXTI_Init(&EXTIInitStructure);
	
	NVICInitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVICInitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC ->IP[23] = 0X30;
	NVIC_EnableIRQ(23); 
	
	NVIC_Init(&NVICInitStructure);
	
}

void EXTI9_5_IRQHandler(){
	
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET){
		for(int i=0; i<10; i++){
			printNumber(portA,i);
			delay(3600000);
		}
		for(int i=8; i>=0; i--){
			printNumber(portA,i);
			delay(3600000);
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}

int main(){
	seg7DisplayInit(portA,PIN0,PIN1,PIN2,PIN3,PIN4,PIN5,PIN6);
	gpioConfig();
	extiConfig();
	
	while(1){
		
		for(int i=0; i<10; i++){
			printNumber(portA,i);
			delay(1800000);
		}
		for(int i=8; i>=0; i--){
			printNumber(portA,i);
			delay(1800000);
		}
	}
}
