#include <M5Stack.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <ClosedCube_TCA9548A.h>

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

void setup()
{
  M5.begin();
  Wire.begin();

  tca9548a.address(PaHub_I2C_ADDRESS);

  for(channel=0; channel < 6; channel++) {
    returnCode = tca9548a.newselectChannel(channel);
    Serial.print("SETUP returnCode:");
    Serial.println(returnCode);
    if (returnCode == 0) {
      Serial.print("SETUP Ch:");
      Serial.println(channel);
    }
    sensor.setTimeout(500);
    if (!sensor.init())
    {
      Serial.println("Failed to detect and initialize sensor!");
      while (1) {}
    }
  }

  sensor.startContinuous();
}

void loop()
{
  for (channel=0; channel < 6; channel++) {
    returnCode = tca9548a.newselectChannel(channel);
    Serial.print("Ch:");
    Serial.print(channel);
    Serial.print(" ");
    if (returnCode == 0) {
      distance = sensor.readRangeSingleMillimeters();
      if (distance < 200) {
        simpleDists[channel] = 0;
      } else if (distance < 600) {
        simpleDists[channel] = 1;
      } else {
        simpleDists[channel] = 2;
      }
      Serial.print(simpleDists[channel]);
      if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
      Serial.println();
    }
  }

  delay(200);
}

