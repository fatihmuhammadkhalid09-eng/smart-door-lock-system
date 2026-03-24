#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// ================= WIFI (ISI SENDIRI) =================
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";

// ================= TELEGRAM =================
String BOTtoken = "YOUR_BOT_TOKEN";
String CHAT_ID = "YOUR_CHAT_ID";

// ================= GLOBAL =================
WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

#define FLASH_LED_PIN 4
bool flashState = LOW;

bool sendPhoto = false;
unsigned long lastTimeBotRan;
const int botRequestDelay = 1000;

// ================= CAMERA PIN (AI THINKER) =================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ================= CAMERA INIT =================
void configInitCamera(){
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn  = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    ESP.restart();
  }
}

// ================= HANDLE TELEGRAM =================
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = bot.messages[i].chat_id;
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Unauthorized", "");
      continue;
    }

    String text = bot.messages[i].text;

    if (text == "/start") {
      String msg = "ESP32-CAM Ready\n";
      msg += "/photo - ambil foto\n";
      msg += "/flash - toggle flash\n";
      msg += "/ip - lihat IP\n";
      bot.sendMessage(chat_id, msg, "");
    }

    if (text == "/flash") {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
    }

    if (text == "/photo") {
      sendPhoto = true;
    }

    if (text == "/ip") {
      bot.sendMessage(chat_id, WiFi.localIP().toString(), "");
    }
  }
}

// ================= SEND PHOTO =================
String sendPhotoTelegram() {

  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera error");
    return "Fail";
  }

  if (!clientTCP.connect("api.telegram.org", 443)) {
    Serial.println("Connection failed");
    return "Fail";
  }

  String head = "--123\r\nContent-Disposition: form-data; name=\"chat_id\";\r\n\r\n" 
                + CHAT_ID + "\r\n--123\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"img.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";

  String tail = "\r\n--123--\r\n";

  uint32_t totalLen = fb->len + head.length() + tail.length();

  clientTCP.println("POST /bot"+BOTtoken+"/sendPhoto HTTP/1.1");
  clientTCP.println("Host: api.telegram.org");
  clientTCP.println("Content-Length: " + String(totalLen));
  clientTCP.println("Content-Type: multipart/form-data; boundary=123");
  clientTCP.println();

  clientTCP.print(head);

  uint8_t *fbBuf = fb->buf;
  size_t fbLen = fb->len;

  for (size_t n = 0; n < fbLen; n += 1024) {
    if (n + 1024 < fbLen) {
      clientTCP.write(fbBuf, 1024);
      fbBuf += 1024;
    } else {
      clientTCP.write(fbBuf, fbLen % 1024);
    }
  }

  clientTCP.print(tail);

  esp_camera_fb_return(fb);
  clientTCP.stop();

  return "OK";
}

// ================= SETUP =================
void setup(){
  Serial.begin(115200);

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, LOW);

  configInitCamera();

  WiFi.begin(ssid, password);
  clientTCP.setInsecure(); // lebih simpel

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());
}

// ================= LOOP =================
void loop() {

  if (sendPhoto) {
    sendPhotoTelegram();
    sendPhoto = false;
  }

  if (millis() - lastTimeBotRan > botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeBotRan = millis();
  }
}