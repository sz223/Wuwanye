#include <Arduino.h>

// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2;

// 函数声明
void blinkBasic();    // 基础闪烁
void blinkSOS();      // SOS信号
void blinkLED(int times, int duration, int gap);

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  // 将LED引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
  Serial.println("ESP32 LED 实验开始！");
  Serial.println("模式: 基础闪烁 → 3秒后切换 SOS");
}

void loop() {
  // ========== 阶段1: 基础闪烁（循环5次）==========
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);   // 点亮LED
    Serial.println("LED ON");     // 串口输出提示
    delay(1000);                  // 保持1秒

    digitalWrite(ledPin, LOW);    // 熄灭LED
    Serial.println("LED OFF");    // 串口输出提示
    delay(1000);                  // 保持1秒
  }

  // 间隔提示
  Serial.println("\n>>> 切换到 SOS 模式 <<<\n");
  delay(2000);

  // ========== 阶段2: SOS 求救信号（循环3次）==========
  for (int i = 0; i < 3; i++) {
    blinkSOS();
    delay(3000);  // 单词间延迟
  }

  delay(3000);
}

// ---------- 基础闪烁（简单封装）----------
void blinkBasic() {
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}

// ---------- SOS 信号 ----------
void blinkSOS() {
  Serial.print("SOS ");
  
  // S: 三短 (.)
  blinkLED(3, 200, 200);
  delay(500);  // 字母间隔
  
  // O: 三长 (-)
  blinkLED(3, 600, 200);
  delay(500);  // 字母间隔
  
  // S: 三短 (.)
  blinkLED(3, 200, 200);
  
  Serial.println("完毕");
}

// ---------- 辅助函数：闪烁指定次数 ----------
// times: 闪烁次数, duration: 亮的时间(ms), gap: 灭的时间(ms)
void blinkLED(int times, int duration, int gap) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(duration);
    digitalWrite(ledPin, LOW);
    delay(gap);
  }
}