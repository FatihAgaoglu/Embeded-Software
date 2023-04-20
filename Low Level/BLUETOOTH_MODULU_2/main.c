#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "math.h"
#include "stdio.h"

char veri=0;
float distance=0;
uint16_t adcValue=0;
uint16_t tempature=0;
char textMessage[25]="";

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//Trig
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	//echo
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_2;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
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
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	
	return ADC_GetConversionValue(ADC1);
	
}

double termistor(uint16_t adcValue){
	double temp;
	temp=log(((40950000 / adcValue) - 10000));
	temp=1/(0.001129148 + (0.000234125 + (0.0000000876741 * temp * temp)) *temp);
	temp=temp-273.15;
	return temp;
	
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
	
	
	while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)));
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){
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

void delay(uint32_t time){
	
	while(time--);
	
}

int main(){
	gpioConfig();
	uartConfig();
	adcConfig();

	while(1){
	
		veri=USART_ReceiveData(USART1);
		
		if(veri=='1'){
			adcValue=readADC();
			tempature=termistor(adcValue);
			sprintf(textMessage,"Sicaklik: %d °C \r\n",tempature);
			uartTransmit(textMessage);
			delay(360000);
		}
		else if (veri=='2'){
			distance=HCSR04_Read();
			sprintf(textMessage,"Uzaklik: %f cm \r\n",distance);
			uartTransmit(textMessage);
			delay(360000);
		}
	}
}
