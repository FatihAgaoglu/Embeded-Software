#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

uint32_t counter=0;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void SysTick_Handler(){
	
	if(counter>0)
		counter--;
	
}

void delay_ms(uint32_t time){
	
	counter = time;
	while(counter);
	
}
	
int main(){
	gpioConfig();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);  //saati 1ms olacak sekilde bold�k.

	while(1){
		
		GPIOB ->BSRR = 0X00000001;
		delay_ms(2000);
		GPIOB ->BSRR = 0X00010000;
		delay_ms(2000);
		
	}
}
