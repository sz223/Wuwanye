#include <Arduino.h>

// ===================== 引脚定义 =====================
#define TOUCH_PIN 4      // 触摸引脚 GPIO4 (T0)
#define LED_PIN   2      // 板载LED
#define CHANNEL   0      // PWM通道

// ===================== 触摸参数 =====================
#define DEBOUNCE_DELAY 200  // 防抖
int threshold = 40;         // 你之前测试的最佳阈值

// ===================== 呼吸灯3档速度配置 =====================
// 数字越小，呼吸越快；数字越大，呼吸越慢
int speedLevel = 1;         // 当前档位：1/2/3
int delayTime[] = {12, 6, 3};// 三档速度：慢、中、快

// ===================== 全局变量 =====================
bool lastTouchState = false;
unsigned long lastDebounceTime = 0;
int brightness = 0;
int fadeStep = 1;

void setup() {
  Serial.begin(115200);
  
  // PWM初始化
  ledcSetup(CHANNEL, 5000, 8);
  ledcAttachPin(LED_PIN, CHANNEL);
}

void loop() {
  // ===================== 1. 触摸调速（档位切换） =====================
  int touchValue = touchRead(TOUCH_PIN);
  bool currentTouch = (touchValue < threshold);

  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    // 触摸瞬间 → 切换档位
    if (currentTouch && !lastTouchState) {
      speedLevel++;
      if (speedLevel > 3) speedLevel = 1; // 循环 1→2→3→1
      
      Serial.print("当前档位：");
      Serial.println(speedLevel);
      
      lastDebounceTime = millis();
    }
  }
  lastTouchState = currentTouch;

  // ===================== 2. 呼吸灯主逻辑 =====================
  brightness += fadeStep;
  
  // 边界反转
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
  }
  
  ledcWrite(CHANNEL, brightness);
  
  // 根据档位延时 → 控制速度
  delay(delayTime[speedLevel - 1]);
}