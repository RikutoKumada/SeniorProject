#include "M5StickC.h"
#include "JoyC.h"
#include "WiFi.h"
#include "esp_now.h"

// 受信側のMACアドレスを設定
uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0xAA, 0xC1, 0x54};

// 送信データの構造体
typedef struct controller_input {
    int angle;
    int distance;
    int x;
    int y;
    int press;
} controller_input;

// インスタンス化
controller_input inputData[2];
JoyC joyc;

esp_now_peer_info_t peerInfo;

// データが送れたかをチェックするコールバック関数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // M5Stick-Cの開始
  M5.begin();
  
  // コントローラのセットアップ
  Wire.begin(0, 26, 400000UL);

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

  // peerの追加
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // M5Stick-Cの更新
  M5.update();

  // コントローラーの入力値を取得
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

  // ディスプレイに入力値を表示
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Angle-L: %d\n", joyc.GetAngle(0));
  M5.Lcd.printf("Dist-L:  %d\n", joyc.GetDistance(0));
  M5.Lcd.printf("X-L:     %d\n", joyc.GetX(0));
  M5.Lcd.printf("Y-L:     %d\n", joyc.GetY(0));
  M5.Lcd.printf("Press-L: %d\n\n", joyc.GetPress(0));
  M5.Lcd.printf("-------------\n\n");
  M5.Lcd.printf("Angle-R: %d\n", joyc.GetAngle(1));
  M5.Lcd.printf("Dist-R:  %d\n", joyc.GetDistance(1));
  M5.Lcd.printf("X-R:     %d\n", joyc.GetX(1));
  M5.Lcd.printf("Y-R:     %d\n", joyc.GetY(1));
  M5.Lcd.printf("Press-R: %d\n", joyc.GetPress(1));

  // ESP-NOWでデータを送信
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &inputData, sizeof(inputData));
  if (result == ESP_OK) {
    Serial.printf("Sent with success");
  }
  else {
    Serial.printf("Error sending the data");
  }

  // 100ms待つ
  delay(100);
}
