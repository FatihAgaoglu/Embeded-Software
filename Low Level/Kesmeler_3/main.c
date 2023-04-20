#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI


void gpioConfig(){
//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC ->APB2ENR = 0X0000001D;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIOB ->CRL = 0X33333333;
	
//	GPIO_Init(GPIOA,&GPIOInitStructure);
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIOB ->CRL = 0X00000333;
		GPIOB ->CRH = 0X00000300;
	
//	GPIO_Init(GPIOB,&GPIOInitStructure);
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_6;
		GPIOC ->CRL = 0X08000800;
	
//	GPIO_Init(GPIOC,&GPIOInitStructure);
	
}

void extiConfig(){
	
//	EXTI_InitTypeDef EXTIInitStructure;
//	NVIC_InitTypeDef NVICInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
		AFIO ->EXTICR[0] = 0X00000200;
		AFIO ->EXTICR[1] = 0X00000200;
	
//	EXTIInitStructure.EXTI_Line=EXTI_Line2 | EXTI_Line6;
//	EXTIInitStructure.EXTI_LineCmd=ENABLE;
//	EXTIInitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
		EXTI ->IMR = 0X00000044;
		EXTI ->RTSR = 0X00000044;
	
//	EXTI_Init(&EXTIInitStructure);
	
	
	//Ust oncelik
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//Öncelik ayar biti 7. bit
	//Bu üstteki parametre girilmezse baslangiç degeri olarak PRIORITYGROUP_4 seçili olacaktir.	7:4 bitleri öncelik ayar bitleri
		
	
//	NVICInitStructure.NVIC_IRQChannel=EXTI2_IRQn;
//	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
//	NVICInitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC ->IP[8] = 0X80;		
		NVIC_EnableIRQ(8);
	
//	NVIC_Init(&NVICInitStructure);
	
//	NVICInitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
//	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVICInitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC ->IP[23] = 0X70;
		NVIC_EnableIRQ(23);
	
	
//	NVIC_Init(&NVICInitStructure);
	
}

void delay(uint32_t time){
	
	while(time--);
	
}

void EXTI2_IRQHandler(){
//	if(EXTI_GetITStatus(EXTI_Line2) == 1){
		if(EXTI ->PR & 0X00000004){
		
		GPIOA ->BSRR = 0X04070000;
		GPIOB ->BSRR = 0X04000007;
		delay(36000000);
		GPIOB ->BSRR = 0X00070000;
	}
//	EXTI_ClearITPendingBit(EXTI_Line2);
		EXTI ->PR = 0X00000004;
}

void EXTI9_5_IRQHandler(){
	
//	if(EXTI_GetITStatus(EXTI_Line6) == 1){
		if(EXTI ->PR & 0X00000040){
			
			GPIOB ->BSRR = 0X00070000;
			
			for(int i=0; i<5; i++){
			GPIOB ->BSRR = 0X00000400;
			delay(3600000);
			GPIOB ->BSRR = 0X04000000;
			delay(3600000);
			}

			GPIOB ->BSRR = 0X00000007;
		
//		GPIOA ->BSRR = 0X0000003F;	//0
//		delay(3600000);
//		GPIOA ->BSRR = 0X003F0030;	//1
//		delay(3600000);
//		GPIOA ->BSRR = 0X0030005B;	//2
//		delay(3600000);
//		GPIOA ->BSRR = 0X005B004F;	//3
//		delay(3600000);
//		GPIOA ->BSRR = 0X004F0066;	//4
//		delay(3600000);
//		GPIOA ->BSRR = 0X0066006D;	//5
//		delay(3600000);
//		GPIOA ->BSRR = 0X006D007D;	//6
//		delay(3600000);
//		GPIOA ->BSRR = 0X007D0007;	//7
//		delay(3600000);
//		GPIOA ->BSRR = 0X0007007F;	//8
//		delay(3600000);
//		GPIOA ->BSRR = 0X007F006F;	//9
//		delay(3600000);
//		GPIOA ->BSRR = 0X006F007F;	//8
//		delay(3600000);
//		GPIOA ->BSRR = 0X007F0007;	//7
//		delay(3600000);
//		GPIOA ->BSRR = 0X0007007D;	//6
//		delay(3600000);
//		GPIOA ->BSRR = 0X007D006D;	//5
//		delay(3600000);
//		GPIOA ->BSRR = 0X006D0066;	//4
//		delay(3600000);
//		GPIOA ->BSRR = 0X0066004F;	//3
//		delay(3600000);
//		GPIOA ->BSRR = 0X004F005B;	//2
//		delay(3600000);
//		GPIOA ->BSRR = 0X005B0030;	//1
//		delay(3600000);
//		GPIOA ->BSRR = 0X0030003F;	//0
//		delay(3600000);
//		GPIOA ->BSRR = 0X00FF0000;
		
	}
//	EXTI_ClearITPendingBit(EXTI_Line6);
		EXTI ->PR = 0X00000040;
}


int main(){
	gpioConfig();
	extiConfig();
	while(1){
		GPIOB ->BSRR = 0X00020001;
		delay(900000);
		GPIOB ->BSRR = 0X00010002;
		delay(900000);
		GPIOB ->BSRR = 0X00020004;
		delay(900000);
		GPIOB ->BSRR = 0X00040002;
		delay(900000);
	}
}
