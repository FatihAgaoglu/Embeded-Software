#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

uint16_t ADCValue=0;
float voltage=0;
float mapValue=0;

void gpioConfig(){
	
		RCC ->APB2ENR = 0X00000004;
	
		GPIOA ->CRL = 0X00000000;
	
}

void adcConfig(){
	
		RCC ->APB2ENR |= 0X00000200;
	
		ADC1 ->CR2 = 0X00000003;	//ADC enable
		ADC1 ->SMPR2 = 0X00000005;	//55.5 Cycles
	
	ADC_Cmd(ADC1,ENABLE);
	
}

uint16_t readADC(){
	
		ADC1 ->CR2 |= 0X00400000;
	
		while(!(ADC1 ->SR & 0X00000010));
	
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
