#include <M5Stack.h>
#include <esp_now.h>
#include <WiFi.h>
#include "bala.h"

Bala bala;

// 受信データの構造体
typedef struct controller_input {
  int angle;
  int distance;
  int x;
  int y;
  int press;
} controller_input;

// 受信データのインスタンス
controller_input inputData[2];

// コールバック関数
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&inputData, incomingData, sizeof(inputData));
}

void setup() {
  M5.begin();

  M5.IMU.Init();
  
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);

  M5.Lcd.begin();
  M5.Lcd.fillScreen(BLUE);           // 画面背景(指定色で画面全体を塗りつぶす。表示を更新する場合にも使用)
  M5.Lcd.setTextColor(WHITE, BLACK); // テキスト色(文字色, 文字背景)
  M5.Lcd.setTextFont(4);              // フォント(フォント番号：0,2,4,6,7,8の中から指定)
  M5.Lcd.setCursor(0, 0);             // テキスト表示座標(x座標, y座標)
  M5.Lcd.setCursor(0, 0, 2);          // テキスト表示座標(x座標, y座標, フォント番号)※フォント指定も可
  M5.Lcd.setTextSize(1);
}

void loop() {
  M5.Lcd.fillScreen(BLUE);  // 画面の塗りつぶし
  M5.Lcd.setCursor(0, 0);  // 文字列の書き出し位置

  // M5.Lcd.print("// LEFT INPUTS //");
  // M5.Lcd.print("Angle: ");
  // M5.Lcd.println(inputData[0].angle);
  // M5.Lcd.print("Distance: ");
  // M5.Lcd.println(inputData[0].distance);
  // M5.Lcd.print("X: ");
  // M5.Lcd.println(inputData[0].x);
  // M5.Lcd.print("Y: ");
  // M5.Lcd.println(inputData[0].y);
  // M5.Lcd.print("Press: ");
  // M5.Lcd.println(inputData[0].press);
  // M5.Lcd.println("");
  // M5.Lcd.print("// RIGHT INPUTS //");
  // M5.Lcd.print("Angle: ");
  // M5.Lcd.println(inputData[1].angle);
  // M5.Lcd.print("Distance: ");
  // M5.Lcd.println(inputData[1].distance);
  // M5.Lcd.print("X: ");
  // M5.Lcd.println(inputData[1].x);
  // M5.Lcd.print("Y: ");
  // M5.Lcd.println(inputData[1].y);
  // M5.Lcd.print("Press: ");
  // M5.Lcd.println(inputData[1].press);

  if (inputData[0].y > 140) {
    bala.SetSpeed(200, 200);
  } else if (inputData[0].y < 60) {
    bala.SetSpeed(-200, -200);
  } else {
    bala.SetSpeed(0, 0);
  }

  delay(100);
}