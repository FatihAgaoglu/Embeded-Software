#include "stm32f10x.h"                  // Device header
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC
#include "stm32f10x_dma.h"              // Keil::Device:StdPeriph Drivers:DMA
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "delay.h"
#include "stdio.h"
#include "lcd.h"

#define SLAVE_ADDRESS 0X08

uint8_t receivedByte;

char textMessage[25]="";

void gpioConfig(){
	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//I2C1_SCL -> PB6 AND I2C1_SDA -> PB7
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
}

void I2Cconfig(){
	
	I2C_InitTypeDef I2CInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	I2CInitStructure.I2C_ClockSpeed=100000;
	I2CInitStructure.I2C_Mode=I2C_Mode_I2C;
	I2CInitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
	I2CInitStructure.I2C_OwnAddress1=0X00;
	I2CInitStructure.I2C_Ack=I2C_Ack_Disable;
	I2CInitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C1,&I2CInitStructure);
	I2C_Cmd(I2C1,ENABLE);
	
}

void I2C_Start(){
	
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));		// I2Cx müsait olana kadar bekle
	
	I2C_GenerateSTART(I2C1,ENABLE);		// Baslat durumunu olustur
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));		
		// I2C EV5 durumu için bekle.
    // Baslangiç durumu düzgün bir sekilde I2C hattina iletilmis demektir
    // (veri hatti bostur ve baska cihazlar iletisimde degildir)
	
}

void I2C_Stop(){
	
	I2C_GenerateSTOP(I2C1,ENABLE);		// I2C durdurma kosulunu olusturur
	
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_STOPF));		// Durdurma kosulu tamamlanana kadar bekle
	
}

void I2C_Address_Direction(uint8_t address, uint8_t direction){
	
	I2C_Send7bitAddress(I2C1,address,direction);		// Slave adresini gönder
	
	if(direction == I2C_Direction_Transmitter){
		
		// I2C EV6 durumu için bekler
    // slave cihaz adresini dogru bir sekilde almis demektir.
		
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		
	}
	else if(direction==I2C_Direction_Receiver){
		
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
		
	}
	
}

void I2C_Transmit(uint8_t byte){
	
	I2C_SendData(I2C1,byte);		// Veriyi gönder
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));		// veri fiziksel olarak veri hatti üzerinde iletilmektedir.
	
}

uint8_t I2C_Receive_ack(){
	
	I2C_AcknowledgeConfig(I2C1,ENABLE);		// Alinan veri için alindi (ACK) bilgisini aktiflestir
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	// I2C EV7 durumunu bekle
  // Bunun anlami veri I2C veri yazmacina alinmistir.
	
	return I2C_ReceiveData(I2C1);			// Veriyi I2C veri yazmacindan oku ve fonksiyondan döndür
	
}

uint8_t I2C_Receive_Nack(){
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);		// Alinan verinin alindi (ACK) bilgisini pasif yap
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	// I2C EV7 durumu için bekle
  // veri I2C veri yazmacina alinmistir.
	
	return I2C_ReceiveData(I2C1);			// Veriyi I2C veri yazmacindan oku ve geri döndür
	
}

void I2C_Write(uint8_t address, uint8_t data){
	
	I2C_Start();
	I2C_Address_Direction(address << 1, I2C_Direction_Transmitter);
	I2C_Transmit(data);
	I2C_Stop();
	
}

void I2C_Read(uint8_t address, uint8_t* data){
	
	I2C_Start();
	I2C_Address_Direction(address << 1, I2C_Direction_Receiver);
	*data=I2C_Receive_Nack();
	I2C_Stop();
	
}

int main(){
	
	DelayInit();
	gpioConfig();
	I2Cconfig();
	lcd_init();
	lcd_clear();
	
	while(1){
		
		I2C_Write(SLAVE_ADDRESS,0X01);		// Arduino borda (slave) 0x01 yaz (LED blink'i baslat)
		delay_ms(5);
		I2C_Read(SLAVE_ADDRESS,&receivedByte);		// LED blinkin durumunu oku (açik mi, kapali mi)
		lcd_clear();
		if(receivedByte==0){
			
			lcd_line1();
			lcd_print("LED Blink Kapali");
			
		}
		else if(receivedByte==1){
			
			lcd_line1();
			lcd_print("LED Blink Acik");
			
		}
		delay_ms(2500);
		
		I2C_Write(SLAVE_ADDRESS,0X00);		// Arduino bord'a (slave) 0x00 yaz (LED blinki kapat)
		delay_ms(5);
		I2C_Read(SLAVE_ADDRESS,&receivedByte);
		lcd_clear();
		if(receivedByte==0){
			
			lcd_line1();
			lcd_print("LED Blink Kapali");
			
		}
		else if(receivedByte==1){
			
			lcd_line1();
			lcd_print("LED Blink Acik");
			
		}
		delay_ms(2500);
	}
}
