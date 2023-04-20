#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

float distance=0;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);

}

void delayus(uint32_t time){
	
	uint32_t newTime=time*24;
	while(newTime--);
	
}

float HCSR04_Read(){
	
	uint32_t time=0;
	float tempDistance=0;
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delayus(10);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	delayus(150);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	delayus(10);
	
	
	while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)));
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		time++;
	}

	tempDistance=(float)(time/2)/29.1;
	
	if(tempDistance>400)
		tempDistance=400;
	if(tempDistance<2) 
		tempDistance=2;
	
	delayus(1000);
	
	return tempDistance;
	
}

int main(){
	
	gpioConfig();
	
	while(1){
		
		distance=HCSR04_Read();
		
	}
}
