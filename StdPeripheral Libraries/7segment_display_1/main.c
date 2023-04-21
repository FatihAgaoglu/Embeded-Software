#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include <stdlib.h>						// Random sayi üretmek için gerekli kütüphane
#include "seg7Display.h"				// 7 segment display kütüphanesi


int main(){
	seg7DisplayInit(portA,PIN0,PIN1,PIN2,PIN3,PIN4,PIN5,PIN6);
	while(1){
		printNumber(portA,5);
		delay(3600000);
		printLetter(portA, 'c');
		delay(3600000);
		cleanDisplay(portA);
		delay(3600000);
		countForward(portA, 3600000);
		delay(3600000);

	}
}