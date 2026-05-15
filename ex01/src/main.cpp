#include <Arduino.h>

#define LED_PIN  2          // 板载 LED 接 GPIO 2

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("LED 1Hz 闪烁开始");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // 亮
  delay(500);                    // 保持 500ms
  digitalWrite(LED_PIN, LOW);    // 暗
  delay(500);                    // 保持 500ms
}