#include <Arduino.h>
#define LED1 4
#define LED2 5
#define CH1 0
#define CH2 1

const int freq = 5000;
const int res = 8;

void setup()
{
  ledcSetup(CH1, freq, res);
  ledcSetup(CH2, freq, res);
  ledcAttachPin(LED1, CH1);
  ledcAttachPin(LED2, CH2);
}

void loop()
{
  for(int i=0;i<=255;i++)
  {
    ledcWrite(CH1,i);
    ledcWrite(CH2,255-i);
    delay(6);
  }
  for(int i=255;i>=0;i--)
  {
    ledcWrite(CH1,i);
    ledcWrite(CH2,255-i);
    delay(6);
  }
}