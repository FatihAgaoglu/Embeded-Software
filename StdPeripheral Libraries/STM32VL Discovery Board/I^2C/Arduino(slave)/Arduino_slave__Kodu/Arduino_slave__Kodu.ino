#include <Wire.h>

#define OWN_ADDRESS  0x08
#define LED_PIN      13

int ledBlink = 0;

void receiveEvent(int bytes) 
{
    // I2C hattındaki veriyi oku
    ledBlink = Wire.read();
}

void requestEvent()
{
    // LED'in durumunu master cihaza gönder
    Wire.write(ledBlink);
}

void setup() 
{
    pinMode (LED_PIN, OUTPUT);
  
    // I2C hattını slave olarak başlat
    Wire.begin(OWN_ADDRESS); 
    // I2C veri hattından veri alındığında tetiklenecek metot
    Wire.onReceive(receiveEvent);
    // I2C veri hattından veri istendiğinde tetiklenecek metot
    Wire.onRequest(requestEvent);
}

void loop() 
{
    // Eğer alınan veri 1 ise 250 ms aralıklarla LED blink yapsın
    if (ledBlink == 1) 
    {
        digitalWrite(LED_PIN, HIGH);
        delay(250);
        digitalWrite(LED_PIN, LOW);
        delay(250);
    }
    // Eğer alınan veri 0 ise LED'i söndür
    else if (ledBlink == 0) 
    {
        digitalWrite(LED_PIN, LOW);
    }
}
