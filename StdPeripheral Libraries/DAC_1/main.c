#include "stm32f10x.h"                  // Device header
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC

uint16_t i;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
}

void dacConfig(){
	
	DAC_InitTypeDef DACInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	DACInitStructure.DAC_LFSRUnmask_TriangleAmplitude=DISABLE;
	DACInitStructure.DAC_OutputBuffer=ENABLE;
	DACInitStructure.DAC_Trigger=DAC_Trigger_None;
	DACInitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_2,&DACInitStructure);
	DAC_Init(DAC_Channel_1,&DACInitStructure);
	DAC_Cmd(DAC_Channel_2,ENABLE);
	DAC_Cmd(DAC_Channel_1,ENABLE);
			
}

void delay(uint32_t time){
		while(time--);
	}

int main(){
	
	gpioConfig();
	dacConfig();
	
	while(1){
		for(i=0; i<=4095; i++){
	//		DAC_SetChannel1Data(DAC_Align_12b_R,i);
	//		DAC_SetChannel2Data(DAC_Align_12b_R,i);
			DAC_SetDualChannelData(DAC_Align_12b_R,i,i);
			delay(36000);
				
		}
		for(i=4095; i>0; i--){
	//		DAC_SetChannel1Data(DAC_Align_12b_R,i);
	//		DAC_SetChannel2Data(DAC_Align_12b_R,i);
			DAC_SetDualChannelData(DAC_Align_12b_R,i,i);
			delay(36000);
				
		}
	}
}
