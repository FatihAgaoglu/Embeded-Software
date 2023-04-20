#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

uint16_t ADCValue=0;
float voltage=0;
float mapValue=0;

void gpioConfig(){
	
//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC ->APB2ENR = 0X00000004;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
		GPIOA ->CRL = 0X00000000;
	
//	GPIO_Init(GPIOA,&GPIOInitStructure);
	
}

void adcConfig(){
	
//	ADC_InitTypeDef ADCInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
		RCC ->APB2ENR |= 0X00000200;
	
//	ADCInitStructure.ADC_ContinuousConvMode=ENABLE;
//	ADCInitStructure.ADC_DataAlign=ADC_DataAlign_Right;
//	ADCInitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
//	ADCInitStructure.ADC_Mode=ADC_Mode_Independent;
//	ADCInitStructure.ADC_NbrOfChannel=1;
//	ADCInitStructure.ADC_ScanConvMode=DISABLE;
		ADC1 ->CR2 = 0X00000003;	//ADC enable
		ADC1 ->SMPR2 = 0X00000005;	//55.5 Cycles
	
//	ADC_Init(ADC1,&ADCInitStructure);
	ADC_Cmd(ADC1,ENABLE);
	
}

uint16_t readADC(){

//	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		ADC1 ->CR2 |= 0X00400000;
	
//	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
		while(!(ADC1 ->SR & 0X00000010));
	
//	return ADC_GetConversionValue(ADC1);
		return ADC1 ->DR;
	
}

float map (float ADCValue, float max, float min, float conMax, float conMin){
	
	return ADCValue*((conMax-conMin)/(max-min));
}

int main(){
	gpioConfig();
	adcConfig();

while(1){
	
	ADCValue=readADC();
	voltage=ADCValue*(2.98/4030);
	mapValue=map(ADCValue,4030,0,180,0);
	
	}
}
