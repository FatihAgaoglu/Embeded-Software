#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

char test[25]= "M.Fatih Agaoglu\r\n";


void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// TX->PA9 AND RX->PA10
	
	//TX
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);

}

void uartConfig(){
	
	USART_InitTypeDef UARTInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	UARTInitStructure.USART_BaudRate=9600;
	UARTInitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	UARTInitStructure.USART_Mode=USART_Mode_Tx;
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


int main(){
	gpioConfig();
	uartConfig();
	
	while(1){
	uartTransmit(test);	
		
	}
}
