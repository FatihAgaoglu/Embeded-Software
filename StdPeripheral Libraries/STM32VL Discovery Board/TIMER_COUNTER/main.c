#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

int counter=0;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void timerConfig(){
	
	TIM_TimeBaseInitTypeDef TIMERInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIMERInitStructure.TIM_ClockDivision=1;
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMERInitStructure.TIM_Period=3999;
	TIMERInitStructure.TIM_Prescaler=5999;
	TIMERInitStructure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM6,&TIMERInitStructure);
	TIM_Cmd(TIM6,ENABLE);
	
}


int main(){
	gpioConfig();
	timerConfig();
	
	while(1){
		
		counter=TIM_GetCounter(TIM6);
		
		if(counter==1999)
			GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
		else if(counter==3999)
			GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);	
	}
}
