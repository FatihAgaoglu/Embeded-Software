#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

uint16_t adcValue[];

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	
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
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
}

void dmaConfig(){
	
	DMA_InitTypeDef DMAInitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_DeInit(DMA1_Channel1);
	
	DMAInitStructure.DMA_BufferSize=1;
	DMAInitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMAInitStructure.DMA_M2M=DISABLE;
	DMAInitStructure.DMA_MemoryBaseAddr=(uint32_t)adcValue;
	DMAInitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMAInitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMAInitStructure.DMA_Mode=DMA_Mode_Circular;
	DMAInitStructure.DMA_PeripheralBaseAddr=(uint32_t) & ADC1->DR;
	DMAInitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMAInitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;;
	DMAInitStructure.DMA_Priority=DMA_Priority_High;
	
	DMA_Init(DMA1_Channel1,&DMAInitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
}

int main(){
	gpioConfig();
	adcConfig();
	dmaConfig();
	
	while(1){
		
		
	}
}
