#include "QLearning.h"
#include <stdint.h>

uint8_t LearnAndReturnAction(uint8_t *dists, uint8_t distsSize, ControllerInput *inputs, uint8_t inputsSize) {
    // ここにQ学習の処理を書く
    return inputs[0].angle;
}