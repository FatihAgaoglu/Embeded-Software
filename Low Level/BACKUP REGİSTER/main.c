#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_iwdg.h"             // Keil::Device:StdPeriph Drivers:IWDG
#include "stm32f10x_pwr.h"              // Keil::Device:StdPeriph Drivers:PWR
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stm32f10x_wwdg.h"             // Keil::Device:StdPeriph Drivers:WWDG
#include <stdio.h>

char buffer[80];
uint16_t counter=0;
uint16_t backup_counter=0;

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// TX->PA9 AND RX->PA10
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
}

void usartConfig(){
	
	USART_InitTypeDef USARTInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USARTInitStructure.USART_BaudRate=9600;
	USARTInitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USARTInitStructure.USART_Mode=USART_Mode_Tx;
	USARTInitStructure.USART_Parity=USART_Parity_No;
	USARTInitStructure.USART_StopBits=USART_StopBits_1;
	USARTInitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&USARTInitStructure);
	USART_Cmd(USART1,ENABLE);
	
}

void usartTransmit(char *string){
	
	while(*string){
		while(!(USART1 ->SR & 0x00000040));
		USART_SendData(USART1,*string);
		*string++;
		
	}
}

void delay(uint32_t time){
	
	while(time--);
	
}

int main(){
	gpioConfig();
	usartConfig();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	while(1){
		
		backup_counter=BKP_ReadBackupRegister(BKP_DR1);
		backup_counter++;
		BKP_WriteBackupRegister(BKP_DR1,backup_counter);
		counter++;
		sprintf(buffer,"NM_DR: %d ||| BKP_DR1: %d \r\n",counter,backup_counter);
		usartTransmit(buffer);
		delay(3600000);
		
	}
}
