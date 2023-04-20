#include "stm32f10x.h"                  // Device header
#include "GPIO_STM32F10x.h"             // Keil::Device:GPIO
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_wwdg.h"             // Keil::Device:StdPeriph Drivers:WWDG


void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void WWDGConfig(){
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_DeInit();
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(120);	
	WWDG_Enable(100);
	
	WWDG_EnableIT();
	
}

void NVICConfig(){
	
	NVIC_InitTypeDef NVICInitStructure;
	
	NVICInitStructure.NVIC_IRQChannel=WWDG_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVICInitStructure.NVIC_IRQChannelSubPriority=0;
	
	NVIC_Init(&NVICInitStructure);	
	
}

void WWDG_IRQHandler(){
	
	WWDG_ClearFlag();
	WWDG_SetCounter(100);
	
	GPIOB->ODR ^= GPIO_Pin_0;
	
	// TIME = (1000/24000000)*4069*PRESCALER*(WINDIW-COUNTER) = 27.3ms 'de bir interrupta girecek
	
}

int main(){
	gpioConfig();
	WWDGConfig();
	NVICConfig();
	
		while(1){
				
			
		}	
}
