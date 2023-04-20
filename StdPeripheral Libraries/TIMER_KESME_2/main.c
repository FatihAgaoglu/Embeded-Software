#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

uint16_t tim3Counter;
uint16_t tim4Counter;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void timerConfig(){
	
	TIM_TimeBaseInitTypeDef TIMERInitStructure;
	NVIC_InitTypeDef NVICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	//TIMER3
	TIMERInitStructure.TIM_ClockDivision=1;
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMERInitStructure.TIM_Period=5999;
	TIMERInitStructure.TIM_Prescaler=999;
	TIMERInitStructure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM3,&TIMERInitStructure);
	TIM_Cmd(TIM3,ENABLE);
	
	//TIMER4
	TIMERInitStructure.TIM_ClockDivision=1;
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMERInitStructure.TIM_Period=5999;
	TIMERInitStructure.TIM_Prescaler=3999;
	TIMERInitStructure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM4,&TIMERInitStructure);
	TIM_Cmd(TIM4,ENABLE);
	
}

void nvicConfig(){
	
	NVIC_InitTypeDef NVICInitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	//TIMER3
	NVICInitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_Init(&NVICInitStructure);
	
	//TIMER4
	NVICInitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	NVIC_Init(&NVICInitStructure);

}

void toggle_led(uint16_t pin){
	
	uint16_t readLed=GPIO_ReadInputDataBit(GPIOB,pin);
	
	if(readLed==(uint16_t)Bit_SET)
		GPIO_ResetBits(GPIOB,pin);
	else 
		GPIO_SetBits(GPIOB,pin);
	
}

void TIM3_IRQHandler(){
	
	toggle_led(GPIO_Pin_0);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	tim3Counter++;
	
}

void TIM4_IRQHandler(){
	
	toggle_led(GPIO_Pin_1);
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	tim4Counter++;
	
}

int main(){
	gpioConfig();
	timerConfig();
	nvicConfig();
	
	while(1){
		
	}
}
