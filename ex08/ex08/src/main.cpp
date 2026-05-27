#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// ====================== 改这里！！！ ======================
const char* ssid = "那就深度吧";
const char* password = "1234567890";
// ==========================================================

WebServer server(80);

// 引脚定义
#define LED_PIN 2
#define TOUCH_PIN T0  // 触摸引脚 T0 (GPIO4)

// 全局状态变量
bool isArmed = false;    // 布防状态：false=撤防, true=布防
bool isAlarming = false; // 报警状态：是否正在狂闪

// 网页HTML
const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>安防报警器</title>
  <style>
    body{text-align:center; margin-top:50px; font-size:24px;}
    .btn{
      padding:20px 40px; font-size:22px; margin:20px;
      border:none; border-radius:10px; cursor:pointer;
    }
    .arm{background:#ff4444; color:white;}
    .disarm{background:#00C851; color:white;}
    .status{margin:30px 0; font-weight:bold;}
  </style>
</head>
<body>
  <h1>物联网安防报警器</h1>
  <div class="status">
    布防状态：%STATE%<br>
    报警状态：%ALARM%
  </div>
  <a href="/arm"><button class="btn arm">布防 (Arm)</button></a>
  <a href="/disarm"><button class="btn disarm">撤防 (Disarm)</button></a>
</body>
</html>
)rawliteral";

// 生成带状态的网页
String getStatusPage() {
  String s = HTML;
  s.replace("%STATE%", isArmed ? "已布防" : "未布防");
  s.replace("%ALARM%", isAlarming ? "⚠️  报警中" : "正常");
  return s;
}

// 主页
void handleRoot() {
  String page = getStatusPage();
  server.send(200, "text/html", page);
}

// 布防
void handleArm() {
  isArmed = true;
  isAlarming = false;
  Serial.println("✅ 系统已布防");
  // 用sendHeader + 302代替redirect，兼容所有WebServer版本
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

// 撤防（同时停止报警）
void handleDisarm() {
  isArmed = false;
  isAlarming = false;
  Serial.println("✅ 系统已撤防，报警停止");
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);

  // 初始化引脚
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // 连接WiFi
  Serial.print("连接WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 连接成功！");
  Serial.print("IP地址: ");
  Serial.println(WiFi.localIP());

  // Web路由
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
  Serial.println("Web服务器已启动！");
}

void loop() {
  server.handleClient();

  // ====================== 报警逻辑 ======================
  if (isArmed && !isAlarming) {
    // 布防状态 → 检测触摸
    if (touchRead(TOUCH_PIN) < 30) { // 手触摸触发
      isAlarming = true;
      Serial.println("⚠️  触摸触发！开始报警！");
    }
  }

  // 报警时LED狂闪
  if (isAlarming) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100); // 高频闪烁
  } else {
    digitalWrite(LED_PIN, LOW); // 不报警就熄灭
  }
}