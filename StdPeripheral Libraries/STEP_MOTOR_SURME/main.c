#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "delay.h"

void gpioConfig(){
	
//	GPIO_InitTypeDef GPIOInitStructure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC->APB2ENR = 0X00000004;
	
//	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
//	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIOB->CRL = 0X00000333;
	
//	GPIO_Init(GPIOA,&GPIOInitStructure);
	
}

void fullStep(){
	
	for(int i=0; i<512; i++){
		
		GPIOA ->BSRR = 0X000E0001;
		delay_ms(2);
		GPIOA ->BSRR = 0X000D0002;
		delay_ms(2);
		GPIOA ->BSRR = 0X000B0004;
		delay_ms(2);
		GPIOA ->BSRR = 0X00070008;
		
		delay_ms(2);
	}
}

void halfStep(){
	
	for(int i=0; i<512; i++){
		
		GPIOA ->BSRR = 0X00060009;
		delay_ms(2);
		GPIOA ->BSRR = 0X000E0001;
		delay_ms(2);
		GPIOA ->BSRR = 0X000C0003;
		delay_ms(2);
		GPIOA ->BSRR = 0X000D0002;
		delay_ms(2);
		GPIOA ->BSRR = 0X00090006;
		delay_ms(2);
		GPIOA ->BSRR = 0X000B0004;
		delay_ms(2);
		GPIOA ->BSRR = 0X0003000C;
		delay_ms(2);
		GPIOA ->BSRR = 0X00070008;
		
		delay_ms(2);
	}
}

void halfStepRotate(int step){
	
	switch(step){
		case 0:
			GPIOA ->BSRR = 0X00060009;
			break;
		case 1:
			GPIOA ->BSRR = 0X000E0001;
			break;
		case 2:
			GPIOA ->BSRR = 0X000C0003;
			break;
		case 3: 
			GPIOA ->BSRR = 0X000D0002;
			break;
		case 4:
			GPIOA ->BSRR = 0X00090006;
			break;
		case 5:
			GPIOA ->BSRR = 0X000B0004;
			break;
		case 6:
			GPIOA ->BSRR = 0X0003000C;
			break;
		case 7:
			GPIOA ->BSRR = 0X00070008;
			break;
		
	}
}

void fullStepRotate(int step){
	
	switch(step){
		case 0:
			GPIOA ->BSRR = 0X000E0001;
			break;
		case 1: 
			GPIOA ->BSRR = 0X000D0002;
			break;
		case 2:
			GPIOA ->BSRR = 0X000B0004;
			break;
		case 3:
			GPIOA ->BSRR = 0X00070008;
			break;
		
	}
}

void setStepper1(float angle, int direction, int speed){
	float perAngle = 0.703125; // 360/512=0.703125(her bir adimdaki açisi)
	int stepNumber = (int) (angle/perAngle);// istenilen açiya gelebilmesi için gereken tur.
	
	for(int seq=0; seq<=stepNumber; seq++){
		
		if(direction==0){
			for(int step=0; step<4; step++){//saat yonunde 
				fullStepRotate(step);
				delay_ms(speed);
			}
		}
		else if(direction==1){// saat yonunun tersi
			for(int step=3; step>=0; step--){
				fullStepRotate(step);
				delay_ms(speed);

			}
		}
	}
}

void setStepper2(float angle, int direction, int speed){
	float perAngle = 0.703125; // 360/512=0.703125(her bir adimdaki açisi)
	int stepNumber = (int) (angle/perAngle);// istenilen açiya gelebilmesi için gereken tur.
	
	for(int seq=0; seq<=stepNumber; seq++){
		
		if(direction==0){
			for(int step=0; step<8; step++){//saat yonunde 
				halfStepRotate(step);
				delay_ms(speed);
			}
		}
		else if(direction==1){// saat yonunun tersi
			for(int step=7; step>=0; step--){
				halfStepRotate(step);
				delay_ms(speed);

			}
		}
	}
}

int main(){
	
	gpioConfig();
	DelayInit();
	
	while(1){
		
		fullStep();
		delay_ms(1000);
		halfStep();
		delay_ms(1000);
		setStepper1(90,1,2);
		delay_ms(1000);
		setStepper1(180,0,2);
		delay_ms(1000);
		setStepper2(135,1,2);
		delay_ms(1000);
		setStepper2(270,0,2);
		delay_ms(1000);
		
	}
}

