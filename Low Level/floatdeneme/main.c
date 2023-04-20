#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART

GPIO_InitTypeDef	GPIO;
USART_InitTypeDef	USART;
int a=4,b=5;
float Katsayi;
char message1[40];

void delay(uint32_t time)
{
	uint32_t ttt=time*72;
	while(ttt--);
}

void gpioconfig()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO.GPIO_Mode		=	GPIO_Mode_AF_PP;		// USART haberlesmesi için portumuzun transmit çikisi
	GPIO.GPIO_Pin		=	GPIO_Pin_9;
	GPIO.GPIO_Speed		=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO);

	GPIO.GPIO_Mode		=	GPIO_Mode_IN_FLOATING;	// USART haberlesmesi için portumuzun receive girisi
	GPIO.GPIO_Pin			=	GPIO_Pin_10;			// Bizim yaptigimiz islemde Stm den arduinoya bilgi aktarimi olacaktir o yüzden receive
	GPIO.GPIO_Speed		=	GPIO_Speed_50MHz;		// baglantisi baglanmaya bilir.
	GPIO_Init(GPIOA,&GPIO);
}

void usartconfig()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART.USART_BaudRate	= 9600;
	USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART.USART_Mode		= USART_Mode_Rx | USART_Mode_Tx;
	USART.USART_Parity		= USART_Parity_No;
	USART.USART_StopBits    = USART_StopBits_1;
	USART.USART_WordLength	= USART_WordLength_8b;

	USART_Init(USART1,&USART);

	USART_Cmd(USART1,ENABLE);
}

void usarttransmit(char *string)
{
	while(*string)
	{
		while(!(USART1->SR &0x00000040));
		USART_SendData(USART1,*string);
		*string++;
	}
}


int main(void)
{
	gpioconfig();
	usartconfig();

	while(1)
	{
		Katsayi=(float)a/b;
		sprintf(message1," %f \r \n ", Katsayi);

		usarttransmit(message1);
		delay(72000);
	}


}
