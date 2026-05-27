#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// 你的热点信息
const char* ssid = "那就深度吧";
const char* password = "1234567890";

WebServer server(80);

#define LED_PIN 4
#define PWM_CH 0

// 网页代码
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>LED调光器</title>
  <style>
    body{text-align:center; margin-top:50px;}
    input{width:300px; height:20px;}
  </style>
</head>
<body>
  <h1>LED 亮度调节</h1>
  <input type="range" min="0" max="255" value="0" id="slider">
  <p>亮度值：<span id="val">0</span></p>

  <script>
    let slider = document.getElementById('slider');
    let val = document.getElementById('val');
    
    slider.oninput = function(){
      val.innerHTML = this.value;
      fetch('/set?brightness='+this.value);
    }
  </script>
</body>
</html>
)rawliteral";

// 处理亮度请求
void handleSet() {
  int bright = server.arg("brightness").toInt();
  ledcWrite(PWM_CH, bright);
  server.send(200, "text/plain", "OK");
}

// 处理主页请求
void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void setup() {
  Serial.begin(115200);
  
  // 初始化LED PWM
  ledcSetup(PWM_CH, 5000, 8);
  ledcAttachPin(LED_PIN, PWM_CH);
  ledcWrite(PWM_CH, 0);

  // 连接WiFi
  Serial.print("正在连接WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // 等待连接，超时30秒
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 60) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi 连接成功！");
    Serial.print("IP地址：");
    Serial.println(WiFi.localIP());

    // 启动网页服务器
    server.on("/", handleRoot);
    server.on("/set", handleSet);
    server.begin();
    Serial.println("网页服务器已启动！");
  } else {
    Serial.println("\n❌ WiFi 连接失败，请检查热点设置！");
  }
}

void loop() {
  server.handleClient();
}