#include <Arduino.h>

// 改成你要的GPIO4（触摸T0）
#define TOUCH_PIN 4    
#define LED_PIN   2    // 板载LED引脚
#define DEBOUNCE_DELAY 200  // 防抖时间，防止手抖误触发

// 阈值根据你之前的情况（触摸低、不触摸高）设置，中间值即可
int threshold = 40;  

bool ledState = false;         // LED当前状态
bool lastTouchState = false;   // 上一次触摸状态
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // 初始状态：LED灭
}

void loop() {
  // 1. 读取触摸值（现在读的是GPIO4）
  int touchValue = touchRead(TOUCH_PIN);
  Serial.print("触摸值: ");
  Serial.println(touchValue);

  // 2. 判断当前是否触摸（触摸值 < 阈值）
  bool currentTouch = (touchValue < threshold);

  // 3. 防抖 + 边缘检测：只在“未触摸→触摸”的瞬间翻转状态
  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    if (currentTouch && !lastTouchState) {
      ledState = !ledState;          // 翻转LED状态
      digitalWrite(LED_PIN, ledState);
      Serial.print("LED状态已切换: ");
      Serial.println(ledState ? "亮" : "灭");
      lastDebounceTime = millis();  // 更新防抖时间
    }
  }

  // 4. 更新上一次触摸状态
  lastTouchState = currentTouch;
}