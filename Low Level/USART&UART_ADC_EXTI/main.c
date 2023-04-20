#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include <stdio.h>

uint32_t Temp=0;
uint32_t ADC_Value=0;
char message[20];

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_4;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	// TX->PA9 AND RX->PA10
	
	//TX
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//RX
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_10;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
}

void adcConfig(){
	
	ADC_InitTypeDef ADCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	ADCInitStructure.ADC_ContinuousConvMode=ENABLE;
	ADCInitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADCInitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADCInitStructure.ADC_Mode=ADC_Mode_Independent;
	ADCInitStructure.ADC_NbrOfChannel=1;
	ADCInitStructure.ADC_ScanConvMode=DISABLE;
	
	ADC_Init(ADC1,&ADCInitStructure);
	ADC_Cmd(ADC1,ENABLE);
	
}

uint32_t Read_ADC(){
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	
	return ADC_GetConversionValue(ADC1);
	
}

float map (float ADC_Value, float max, float min, float conMax, float conMin){
	
	return ADC_Value*((conMax-conMin)/(max-min));
	
}

void delay(uint32_t time){
	
	while(time--);
	
}

void extiConfig(){
	
	EXTI_InitTypeDef EXTIInitStructure;
	NVIC_InitTypeDef NVICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
	
	EXTIInitStructure.EXTI_Line=EXTI_Line0 | EXTI_Line4;
	EXTIInitStructure.EXTI_LineCmd=ENABLE;
	EXTIInitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTIInitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	
	EXTI_Init(&EXTIInitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
	
	NVICInitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVICInitStructure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVICInitStructure);
	
	NVICInitStructure.NVIC_IRQChannel=EXTI4_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVICInitStructure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVICInitStructure);
	
}

void uartConfig(){
	
	USART_InitTypeDef UARTInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	UARTInitStructure.USART_BaudRate=9600;
	UARTInitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	UARTInitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	UARTInitStructure.USART_Parity=USART_Parity_No;
	UARTInitStructure.USART_StopBits=USART_StopBits_1;
	UARTInitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&UARTInitStructure);
	USART_Cmd(USART1,ENABLE);
	
}

void uartTransmit(char *string){
	
	while(*string){
		while(!(USART1 ->SR & 0X00000040));
		USART_SendData(USART1,*string);
		*string++;
	}
}

void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) == 1){
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		delay(36000000);
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI4_IRQHandler(){
		if(EXTI_GetITStatus(EXTI_Line4) == 1){
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		delay(36000000);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

int main(){
	gpioConfig();
	extiConfig();
	adcConfig();
	uartConfig();
	
	while(1){
		
		ADC_Value=Read_ADC();
		Temp=map(ADC_Value,4030,0,240,0);
		sprintf(message,"adcValue=%d\r\n",Temp);
		uartTransmit(message);
		delay(360000);
		
		if(Temp<20){
			GPIO_SetBits(GPIOA,GPIO_Pin_4);
			delay(360);
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		}
		else{
			GPIO_ResetBits(GPIOA,GPIO_Pin_4);
			delay(360);
		}
	}
}
