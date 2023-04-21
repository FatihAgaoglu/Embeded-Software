#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stdio.h"
#include "lcd.h"
#include "delay.h"
#include "math.h"

int encoder_value=0;
int Encoder_TIM=0;
char textMessage1[25]="";
char textMessage2[25]="";
uint16_t potValue=0;
uint16_t Duty_cycle=0;


void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	
	//Motor PWM Sinyal
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//Motor Yön Kontrol
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin= GPIO_Pin_0 | GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	//ADC Okumasý
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//Encoder
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void timerConfig(){
	
	TIM_TimeBaseInitTypeDef TIMERInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIMERInitStructure.TIM_ClockDivision=1;
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMERInitStructure.TIM_Prescaler=10;
	TIMERInitStructure.TIM_Period=4099;
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

uint16_t readADC(){
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	
	return ADC_GetConversionValue(ADC1);
	
}

void Encoder_Init_TIM4(){
	
	TIM_TimeBaseInitTypeDef TIMERInitStructure;
	TIM_ICInitTypeDef TIMERICInitStructure;
	NVIC_InitTypeDef NVICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	TIMERInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIMERInitStructure.TIM_Prescaler=0;
	TIMERInitStructure.TIM_Period=65535;
	
	TIM_TimeBaseInit(TIM4,&TIMERInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIMERICInitStructure.TIM_ICFilter=10;
	
	TIM_ICInit(TIM4,&TIMERICInitStructure);
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 
	TIM_SetCounter(TIM4,0);	
	TIM_Cmd(TIM4,ENABLE);
	
	NVICInitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&NVICInitStructure);

}

int Read_Enkoder_Value(){
	
	Encoder_TIM = TIM_GetCounter(TIM4);
	TIM4->CNT=0;
	return Encoder_TIM;
	
}

void goForward (GPIO_TypeDef* port, uint16_t forwardPin, uint16_t backwardPin,uint16_t potValue){
	
	GPIO_SetBits(port,forwardPin);
	GPIO_ResetBits(port,backwardPin);
	pwmConfig(potValue);
	
}

void goBackward (GPIO_TypeDef* port, uint16_t forwardPin, uint16_t backwardPin,uint16_t potValue){
	
	GPIO_SetBits(port,backwardPin);
	GPIO_ResetBits(port,forwardPin);
	pwmConfig(potValue);
	
}

void stop (GPIO_TypeDef* port, uint16_t forwardPin, uint16_t backwardPin){
	
	GPIO_ResetBits(port,backwardPin | forwardPin);
	
}

void delay(uint32_t time){
	
	while(time--);
	
}

float map(float potValue, float max , float min ){
	
	return 100*((potValue+1)/4099);
	
}

int main(){
	
	gpioConfig();
	timerConfig();
	adcConfig();
	Encoder_Init_TIM4();
	lcd_init();
	lcd_clear();
	
	while(1){
		
		potValue=readADC();
		goForward(GPIOB,GPIO_Pin_0,GPIO_Pin_1,potValue);
//		goBackward(GPIOB,GPIO_Pin_0,GPIO_Pin_1,potValue);
		Duty_cycle=map(potValue,4050,50);
		encoder_value = Read_Enkoder_Value();
		lcd_line1();
		sprintf(textMessage1," Duty Cycle:%d ",Duty_cycle);
		lcd_print(textMessage1);
		Delay(300);
//		lcd_line2();
//		sprintf(textMessage2," ADC Deger:%d ",potValue);
//		lcd_print(textMessage2);
//		Delay(300);	
		lcd_line2();
		sprintf(textMessage2," Hiz:%d dev/dk ",encoder_value/5);
		lcd_print(textMessage2);
		Delay(200);
	}
}
