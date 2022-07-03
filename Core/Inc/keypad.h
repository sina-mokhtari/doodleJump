//
// Created by Sina on 6/27/2022.
//

#ifndef DOODLEJUMP_KEYPAD_H
#define DOODLEJUMP_KEYPAD_H

#include "requisite.h"

extern GPIO_TypeDef *const rowPorts[];
extern const uint16_t rowPins[];
extern GPIO_TypeDef *const columnPorts[];
extern const uint16_t columnPins[];
extern uint32_t lastGpioExti;

void keypadAssign(uint16_t gpioPin);
void keypadHandle();

#endif //DOODLEJUMP_KEYPAD_H
