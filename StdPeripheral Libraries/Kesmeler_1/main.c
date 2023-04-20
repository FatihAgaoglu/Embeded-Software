#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

void gpioConfig(){
	
//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC ->APB2ENR = 0X00000019; 				//AFIO ayarini da burda yaptik.
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIOB ->CRL = 0X00000333;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);
//	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIOC ->CRL = 0X08000000;
	
//	GPIO_Init(GPIOC, &GPIOInitStructure);
	
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
//	EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	
		EXTI ->IMR = 0X00000040;
		EXTI ->EMR = 0X00000000;
		EXTI ->FTSR = 0X00000000;
		EXTI ->RTSR = 0X00000040;
		EXTI ->SWIER = 0X00000000;
	
//	EXTI_Init(&EXTIInitStructure);
	
//	NVICInitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
//	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVICInitStructure.NVIC_IRQChannelSubPriority=1;
	
		NVIC ->IP[23] = 0X30;
		NVIC_EnableIRQ(23); 					//NVIC ->ISER[23>>30]=1<< (23 & 0X1F);

//	NVIC_Init(&NVICInitStructure);
	
}

void delay (uint32_t time){
	
	while (time--);
	
}

void EXTI9_5_IRQHandler(){
	
	//if(EXTI_GetITStatus(EXTI_Line6)!=RESET){
		if(EXTI->PR & 0X00000040){
		
		for(int i=0; i<4; i++){
		//	GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			GPIOB ->BSRR = 0X00000007;
			delay(3600000);
		//	GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			GPIOB ->BSRR = 0X00070000;
			delay(3600000);
			
		}
	//	EXTI_ClearITPendingBit(EXTI_Line6);
			EXTI ->PR = 0X00000040;
	}
}

int main(){
	
	gpioConfig();
	extiConfig();
//	int ledArray[3]={GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_2};
	
	while(1){
		
		
//	for(int i=0;i<3;i++){
	//	GPIO_SetBits(GPIOB,ledArray[i]);
		GPIOB ->BSRR = 0X00060001;
		delay(3600000);
	//	GPIO_ResetBits(GPIOB,ledArray[i]);
		GPIOB ->BSRR = 0X00050002;
		delay(3600000);
		GPIOB ->BSRR = 0X00030004;
//		}
		
//		for(int i=1;i>0;i--){
//		GPIO_SetBits(GPIOB,ledArray[i]);
		delay(3600000);
//		GPIO_ResetBits(GPIOB,ledArray[i]);
		GPIOB ->BSRR = 0X00050002;
		delay(3600000);
//		}
	}
}
