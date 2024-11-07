#include <M5Stack.h>
// TOFDists
#include <Wire.h>
#include <VL53L0X.h>
#include <ClosedCube_TCA9548A.h>
// ReceiveWirelessData
#include <WiFi.h>
#include <esp_now.h>
#include "bala.h"
// C2INO
#include "QLearning.h"

// TOFDists
VL53L0X sensor;
ClosedCube::Wired::TCA9548A tca9548a;
#define PaHub_I2C_ADDRESS 0x70
#define BUFFER_SIZE  16
#define SHT_addr 0x44
byte buffer[BUFFER_SIZE];
uint8_t returnCode = 0;
uint8_t channel = 0;
uint8_t simpleDists[6];
int distance = 0;

Bala bala;
ControllerInput inputData[2];

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) { // データを受け取るコールバック関数
  memcpy(&inputData, incomingData, sizeof(inputData));
}

uint8_t action; // 動作

void setup()
{
  // M5Stackの開始
  M5.begin();
  M5.IMU.Init();

  // TOFDistsのセットアップ
  Wire.begin();
  tca9548a.address(PaHub_I2C_ADDRESS);
  for(channel=0; channel < 6; channel++) {
    returnCode = tca9548a.newselectChannel(channel);
    sensor.setTimeout(500);
    if (!sensor.init())
    {
      Serial.println("Failed to detect and initialize sensor!");
      while (1) {}
    }
  }
  sensor.startContinuous();

  // ReceiveWirelessDataのセットアップ
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  // TOFDists
  for (channel=0; channel < 6; channel++) {
    returnCode = tca9548a.newselectChannel(channel);
    if (returnCode == 0) {
      distance = sensor.readRangeSingleMillimeters();
      if (distance < 200) {
        simpleDists[channel] = 0;
      } else if (distance < 600) {
        simpleDists[channel] = 1;
      } else {
        simpleDists[channel] = 2;
      }
      if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    }
  }

  // C言語で強化学習を行い、行動を取得
  action = LearnAndReturnAction(simpleDists, 6, inputData, 2);

  // ロボットの動作を制御する
  // switch (action) {
  //   case: 0: // forward
  //     bala.SetSpeed(100, 100);
  //   case 1: // left
  //     bala.SetSpeed(100, 0);
  //   case 2: // right
  //     bala.SetSpeed(0, 100);
  //   case 3: // stop
  //     bala.SetSpeed(0, 0);
  // }
    
  delay(200);
}
