#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// ===================== 你的WiFi信息 =====================
const char* ssid = "那就深度吧";
const char* password = "1234567890";
// ======================================================

WebServer server(80);

// 触摸引脚
#define TOUCH_PIN T0

// 网页主页
const char HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>实时触摸传感器仪表盘</title>
  <style>
    body {
      text-align: center;
      font-family: Arial;
      background: #1a1a1a;
      color: white;
      margin-top: 100px;
    }
    .title {
      font-size: 32px;
      margin-bottom: 40px;
    }
    .value {
      font-size: 80px;
      font-weight: bold;
      color: #00ff99;
    }
    .tip {
      margin-top: 30px;
      font-size: 18px;
      color: #ccc;
    }
  </style>
</head>
<body>
  <div class="title">ESP32 实时触摸传感器数值</div>
  <div class="value" id="touchValue">0</div>
  <div class="tip">手靠近 → 数值变小 | 手离开 → 数值恢复</div>

  <script>
    // AJAX 每 100ms 请求一次最新数值
    function updateValue() {
      var xhr = new XMLHttpRequest();
      xhr.open('GET', '/data', true);
      xhr.onload = function() {
        if (xhr.status == 200) {
          document.getElementById('touchValue').innerText = xhr.responseText;
        }
      };
      xhr.send();
    }
    setInterval(updateValue, 100);
  </script>
</body>
</html>
)rawliteral";

// 返回网页
void handleRoot() {
  server.send(200, "text/html", HTML);
}

// 返回纯传感器数值
void handleData() {
  int val = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(val));
}

void setup() {
  Serial.begin(115200);

  // WiFi 连接
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // 路由
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}