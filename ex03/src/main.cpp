#include <Arduino.h>  // 必须包含，解决Serial、delay未定义的问题

const int ledPin = 2;
const int channel = 0;
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);
  // 旧版写法，兼容所有ESP32 Arduino版本，避免ledcAttach未定义
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(ledPin, channel);
}

void loop() {
  // 变亮
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(channel, dutyCycle);
    delay(10);
  }
  // 变暗
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(channel, dutyCycle);
    delay(10);
  }
  Serial.println("Breath cycle done");
}