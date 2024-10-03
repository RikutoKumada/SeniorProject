#include "M5StickC.h"
#include "JoyC.h"
#include "esp_now.h"
#include "WiFi.h"

// コントローラーの変数設定
JoyC joyc;
TFT_eSprite img = TFT_eSprite(&M5.Lcd);

// 受信側のMACアドレス
uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0xAA, 0xC1, 0x54}; // 受信側のMACアドレスを設定

// 送信データの構造体
typedef struct controller_input {
    int angle;
    int distance;
    int x;
    int y;
    int press;
} controller_input;

// 送信データのインスタンス
controller_input inputData[2];

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  M5.begin();
  
  // コントローラのセットアップ
  Wire.begin(0, 26, 400000UL);
  img.createSprite(80, 160);

  // ESP_NOWのセットアップ
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  M5.update();

  // M5.Lcd.fillScreen(BLUE);  // 画面の塗りつぶし
  // M5.Lcd.setCursor(0, 0);  // 文字列の書き出し位置

  // Set values to send
  inputData[0].angle = joyc.GetAngle(0);
  inputData[0].distance = joyc.GetDistance(0);
  inputData[0].x = joyc.GetX(0);
  inputData[0].y = joyc.GetY(0);
  inputData[0].press = joyc.GetPress(0);
  inputData[1].angle = joyc.GetAngle(1);
  inputData[1].distance = joyc.GetDistance(1);
  inputData[1].x = joyc.GetX(1);
  inputData[1].y = joyc.GetY(1);
  inputData[1].press = joyc.GetPress(1);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &inputData, sizeof(inputData));
  if (result == ESP_OK) {
    Serial.printf("Sent with success");
  }
  else {
    Serial.printf("Error sending the data");
  }

  delay(100);
}