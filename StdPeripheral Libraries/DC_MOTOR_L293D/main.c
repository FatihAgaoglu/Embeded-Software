#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

#define portA GPIOA
#define portB GPIOB
#define portC GPIOC

#define pin0  (uint16_t)GPIO_Pin_0
#define pin1  (uint16_t)GPIO_Pin_1
#define pin2  (uint16_t)GPIO_Pin_2
#define pin3  (uint16_t)GPIO_Pin_3
#define pin4  (uint16_t)GPIO_Pin_4
#define pin5  (uint16_t)GPIO_Pin_5
#define pin6  (uint16_t)GPIO_Pin_6
#define pin7  (uint16_t)GPIO_Pin_7
#define pin8  (uint16_t)GPIO_Pin_8
#define pin9  (uint16_t)GPIO_Pin_9
#define pin10 (uint16_t)GPIO_Pin_10
#define pin11 (uint16_t)GPIO_Pin_11
#define pin12 (uint16_t)GPIO_Pin_12
#define pin13 (uint16_t)GPIO_Pin_13
#define pin14 (uint16_t)GPIO_Pin_14
#define pin15 (uint16_t)GPIO_Pin_15


void motorInit(GPIO_TypeDef* port, uint16_t forwardPin, uint16_t backwardPin){
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	if(port==portA){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		
	}
	else if(port==portB){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	}
	else if(port==portC){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	}
	else {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	}
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin= forwardPin | backwardPin;
	GPIOInitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(port,&GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	TIM_TimeBaseInitTypeDef TIMERInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIMERInitStructure.TIM_ClockDivision=1;
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMERInitStructure.TIM_Period=2399;
	TIMERInitStructure.TIM_Prescaler=10;
	TIMERInitStructure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TIM2,&TIMERInitStructure);
	TIM_Cmd(TIM2,ENABLE);
	
}

void pwmConfig(uint32_t timPulse){
	
	TIM_OCInitTypeDef TIMER_OCInitStructure;
	
	TIMER_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMER_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIMER_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMER_OCInitStructure.TIM_Pulse=timPulse;
	
	TIM_OC1Init(TIM2,&TIMER_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

}

float map (float ADCValue, float max, float min, float conMax, float conMin){
	
	return ADCValue*((conMax-conMin)/(max-min));
}

void goForward (GPIO_TypeDef* port, uint16_t forwardPin, uint16_t backwardPin, uint16_t speed){
	
	GPIO_SetBits(port,forwardPin);
	GPIO_ResetBits(port,backwardPin);
	pwmConfig(map(speed,255,0,2399,0));
	
}

void goBackward (GPIO_TypeDef* port, uint16_t forwardPin, uint16_t backwardPin, uint16_t speed){
	
	GPIO_SetBits(port,backwardPin);
	GPIO_ResetBits(port,forwardPin);
	pwmConfig(map(speed,255,0,2399,0));
	
}

void stop (GPIO_TypeDef* port, uint16_t forwardPin, uint16_t backwardPin){
	
	GPIO_ResetBits(port,backwardPin | forwardPin);
	
}

void delay(uint32_t time){
	
	while(time--);
	
}

int main(){
	
		motorInit(portB,pin0,pin1);
	
	while(1){
		
		goForward(portB,pin0,pin1,200);
		delay(7200000);
		stop(portB,pin0,pin1);
		delay(7200000);
		goBackward(portB,pin0,pin1,200);
		delay(7200000);
		stop(portB,pin0,pin1);
		delay(7200000);
		
		
	}
}
