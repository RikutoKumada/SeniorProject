#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

// 受信データの構造体
typedef struct {
  uint16_t angle;
  uint8_t distance;
  uint8_t x;
  uint8_t y;
  uint8_t press;
} ControllerInput;

// 関数のプロトタイプ宣言
uint8_t LearnAndReturnAction(uint8_t *dists, uint8_t distsSize, ControllerInput *inputs, uint8_t inputsSize); 

#ifdef __cplusplus
}
#endif

#endif