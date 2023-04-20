#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "stdio.h"


char textMessage[25]="";
uint16_t potValue=0;


void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//GPIOA Portu Aktif Edildi.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//GPIOB Portu Aktif Edildi.
	
		//Motor PWM Sinyal
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	//PWM i�in Alternative Function Push Pull Se�ildi.
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);	//A Portu 'na gerekli ayarlari y�kl�yoruz.
	
	
	//Motor �alisma
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		//Motor �alisma i�in Out Push Pull ayarlandi.
	GPIOInitStructure.GPIO_Pin= GPIO_Pin_0 | GPIO_Pin_1;
	GPIOInitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);		//B Portu 'na gerekli ayarlari y�kl�yoruz.
	
	
	//ADC INPUT
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AIN;	//Analog Input olarak mod ayarlanir.
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_1;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
	
	// USART1 i�in TX->PA9 (Transmit)
	
	//TX
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	//Veriyi g�ndermek i�in Alternative Function olarak ayarlandi.
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOInitStructure);
	
}

void uartConfig(){
	
	USART_InitTypeDef UARTInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);		//USART1 Aktif Edildi.
	
	UARTInitStructure.USART_BaudRate=9600;	
	UARTInitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//Kontrol yapilmadigi i�in None se�ildi
	UARTInitStructure.USART_Mode=USART_Mode_Tx;		//Sadece veri g�nderimi oldugu i�in Tx se�ildi.
	UARTInitStructure.USART_Parity=USART_Parity_No;		//Parity biti kullanilmadi.
	UARTInitStructure.USART_StopBits=USART_StopBits_1;		//Stop bit 1 se�ildi.
	UARTInitStructure.USART_WordLength=USART_WordLength_8b;		//Veri uzunlugumuz 8 bit uzunlugundadir.
	
	USART_Init(USART1,&UARTInitStructure);		//USART1'in gerekli ayarlari y�kl�yoruz.
	USART_Cmd(USART1,ENABLE);		//USART1 'in baslamasi i�in gerekli komutu verdik.
	
}
//Mesajin g�nderilmesi i�in gerekli fonksiyon yazildi.
void uartTransmit(char *string){
	
	while(*string){
		
		while(!(USART1 ->SR & 0X00000040));
		USART_SendData(USART1,*string);
		*string++;
		
	}
}

void adcConfig(){
	
	ADC_InitTypeDef ADCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);		//ADC1 birimi APB2 bus hattindan aktif edildi.
	
	ADCInitStructure.ADC_ContinuousConvMode=ENABLE;		//S�rekli okuma yapilacagi i�in ContinuousConvMode aktif edildi.
	ADCInitStructure.ADC_DataAlign=ADC_DataAlign_Right;		//Veriler saga hizalanacak.
	ADCInitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;		//Tetikleme olmadan �alisacagi i�in None se�ildi.
	ADCInitStructure.ADC_Mode=ADC_Mode_Independent;		//Bagimsiz Modda �alisacagi i�in Independt se�ildi.
	ADCInitStructure.ADC_NbrOfChannel=1;		// 1 ADC d�n�s�m� yapildigi i�in 1 yazildi.
	ADCInitStructure.ADC_ScanConvMode=DISABLE;		//�oklu ADC okumasi yapilmadigi i�in DISABLE se�ildi.
	
	ADC_Init(ADC1,&ADCInitStructure);			//ADC1' i�in gerekli ayarlari y�kl�yoruz.
	ADC_Cmd(ADC1,ENABLE);		//ADC1 'in baslamasi i�in gerekli komutu verdik.
	
}

uint16_t readADC(){
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
	//ADC1 'in 1.kanalini kullandigimizi belirtip �rnekleme zamanini ayarliyoruz.
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	//ADC1 'i baslatiyoruz.
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	
	return ADC_GetConversionValue(ADC1);		// Degeri D�nd�r�yoruz.
	
}

void timerConfig(){
	
	TIM_TimeBaseInitTypeDef TIMERInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);		//TIM2 birimini APB1 Bus hattindan aktif ediyoruz.
	
  TIMERInitStructure.TIM_ClockDivision=1;		//72Mhz hizini b�lm�yoruz.
	TIMERInitStructure.TIM_CounterMode=TIM_CounterMode_Up;		//Yukari saymasi i�in ayarliyoruz.
	TIMERInitStructure.TIM_Prescaler=10;	//Prescaler 10 olarak ayarlandi.
	TIMERInitStructure.TIM_Period=4099;		//Period 4099 ayarlandi.
	TIMERInitStructure.TIM_RepetitionCounter=0;		//Repetition Counter Advanced Timerlarda kullaniliyor.Bundan dolayi 0 ayarlandi.
	
	TIM_TimeBaseInit(TIM2,&TIMERInitStructure);		//TIM2 birimine gerekli ayarlamalar y�klendi.
	TIM_Cmd(TIM2,ENABLE);		//TIM2 birimini baslatiyoruz.

}
//TIM2 de ayarlanan frekansi PWM sinyal olarak alip Duty Cycle degeri ile motoru s�r�yoruz.
void pwmConfig(uint32_t timPulse){
	
	TIM_OCInitTypeDef TIMER_OCInitStructure;
	
	TIMER_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIMER_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIMER_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIMER_OCInitStructure.TIM_Pulse=timPulse;		//Duty Cycle degerini girilen yer.

  TIM_OC1Init(TIM2,&TIMER_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

}

//	Belirli bir gecikmeyi olusturmak i�in gerekli fonksiyon
void delay(uint32_t time){
	
	while(time--);
	
}

int main(){
	
	//Ayarlamalarini yaptigimiz birimlerin fonksiyonlarini ana fonksiyonumuzda �agiriyoruz.
	gpioConfig();
	uartConfig();
	adcConfig();
	timerConfig();
	
	while(1){
		
		potValue=readADC();		//Okunan ADC degerini PotValue degiskenine aktariyoruz.
		GPIO_SetBits(GPIOB,GPIO_Pin_0);		//Motorun d�nmesi i�in PB0 pinini aktif ediyoruz.
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);		//Motorun D�nmesi i�in PB1 pinini pasit ediyoruz.
		pwmConfig(potValue);		//PotValue degerini PwmConfig fonksiyonuna timPulse parametresine y�kl�yoruz.
		sprintf(textMessage,"ADC Deger: %d \r\n",potValue);		//ADC degerini yazdiriyoruz.
		uartTransmit(textMessage);		//Yazdirilan mesaji textMessage adinda olusturdugumuz dizi ile bilgisayara g�nderiyoruz.
		delay(360000);
		
		
	}
}
