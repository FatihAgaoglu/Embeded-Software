#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

//sin signal
uint16_t sinArray[64]={2048,2248,2447,
2642,2831,3013,3185,3346,3495,3630,
3750,3853,3939,4007,4056,4085,4095,
4085,4056,4007,3939,3853,3750,3630,
3495,3346,3185,3013,2831,2642,2447,
2248,2048,1847,1648,1453,1264,1082,
910,749,600,465,345,242,156,88,39,
10,0,10,39,88,156,242,345,465,600,
749,910,1082,1264,1453,1648,1847};

//sawtooth signal
uint16_t sawArray[64]={82,1065,2539,
4095,3600,3550,3500,3450,3400,3350,
3300,3250,3200,3150,3100,3050,3000,
2950,2900,2850,2800,2750,2700,2650,
2600,2550,2500,2450,2400,2234,2179,
2128,2085,2010,1936,1861,1800,1787,
1712,1638,1564,1489,1415,1360,1300,
1266,1191,1117,1042,968,893,819,745,
670,596,521,447,372,300,298,223,149,100,50};

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
	DACInitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Enable;
	DACInitStructure.DAC_Trigger=DAC_Trigger_None;
	DACInitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_1,&DACInitStructure);
	DAC_Init(DAC_Channel_2,&DACInitStructure);
	DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_Cmd(DAC_Channel_2,ENABLE);
	
}

void delay(uint32_t time){
	while(time--);
}

int main(){
	gpioConfig();
	dacConfig();
	
while(1){
		for(int i=0; i<64; i++){
			DAC_SetChannel1Data(DAC_Align_12b_R,sinArray[i]); //sin wave
			delay(9000);			
			DAC_SetChannel2Data(DAC_Align_12b_R,sawArray[i]); //sawtooth wave
			delay(18000);
		}
	}
}
