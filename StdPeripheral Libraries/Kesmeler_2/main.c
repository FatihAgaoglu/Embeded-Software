#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

uint16_t counter=0;

void gpioConfig(){
	
//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC ->APB2ENR = 0X00000019;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIOB ->CRL = 0X00000333;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
		GPIOC ->CRL = 0X08000000;
	
//	GPIO_Init(GPIOC, &GPIOInitStructure);
	
}

void delay (uint32_t time){
	
	while (time--);
	
}

void extiConfig(){
	
//	EXTI_InitTypeDef EXTIInitStructure;
//	NVIC_InitTypeDef NVICInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
		AFIO ->EXTICR[1] = 0X00000200;
	
//	EXTIInitStructure.EXTI_Line=EXTI_Line6;
//	EXTIInitStructure.EXTI_LineCmd=ENABLE;
//	EXTIInitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	
		EXTI ->IMR = 0X00000040;
		EXTI ->RTSR = 0X00000040;
	
//	EXTI_Init(&EXTIInitStructure);
	
//	NVICInitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
//	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVICInitStructure.NVIC_IRQChannelSubPriority=1;

		NVIC ->IP[23] = 0X30;
		NVIC_EnableIRQ(23); 					//NVIC ->ISER[23>>30]=1<< (23 & 0X1F);
	
//	NVIC_Init(&NVICInitStructure);
	
}

void EXTI9_5_IRQHandler(){
	
	//	if(EXTI_GetITStatus(EXTI_Line6) != RESET){
			if(EXTI->PR & 0X00000040){
			counter++;
			GPIOB ->BSRR = 0X00070000;
			delay(3600000);
		}
//	EXTI_ClearITPendingBit(EXTI_Line6);
		EXTI ->PR = 0X00000040;
}

int main(){
	gpioConfig();
	extiConfig();
	
	while(1){
		
//		GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			GPIOB ->BSRR = 0X00000007;
		delay(360000);
//		GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			GPIOB ->BSRR = 0X00070000;
		delay(360000);
	}	
}
