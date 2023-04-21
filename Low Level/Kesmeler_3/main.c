#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI


void gpioConfig(){
	RCC ->APB2ENR = 0X0000001D;
	GPIOB ->CRL = 0X33333333;
	
	GPIOB ->CRL = 0X00000333;
	GPIOB ->CRH = 0X00000300;
	
	GPIOC ->CRL = 0X08000800;
	
}

void extiConfig(){
	
	AFIO ->EXTICR[0] = 0X00000200;
	AFIO ->EXTICR[1] = 0X00000200;
	
	EXTI ->IMR = 0X00000044;
	EXTI ->RTSR = 0X00000044;
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//Öncelik ayar biti 7. bit
	//Bu üstteki parametre girilmezse baslangiç degeri olarak PRIORITYGROUP_4 seçili olacaktir.	7:4 bitleri öncelik ayar bitleri
		
	
	NVIC ->IP[8] = 0X80;		
	NVIC_EnableIRQ(8);
	
	NVIC ->IP[23] = 0X70;
	NVIC_EnableIRQ(23);
	
}

void delay(uint32_t time){
	
	while(time--);
	
}

void EXTI2_IRQHandler()
{
	if(EXTI ->PR & 0X00000004){	
	GPIOA ->BSRR = 0X04070000;
	GPIOB ->BSRR = 0X04000007;
	delay(36000000);
	GPIOB ->BSRR = 0X00070000;
	}
	EXTI ->PR = 0X00000004;
}

void EXTI9_5_IRQHandler()
{
	if(EXTI ->PR & 0X00000040){
			
		GPIOB ->BSRR = 0X00070000;	
		for(int i=0; i<5; i++){
		GPIOB ->BSRR = 0X00000400;
		delay(3600000);
		GPIOB ->BSRR = 0X04000000;
		delay(3600000);
		}
	GPIOB ->BSRR = 0X00000007;
	}
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
