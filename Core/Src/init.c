//
// Created by Sina on 6/27/2022.
//
#include "requisite.h"
#include "keypad.h"
#include "buzzer.h"
#include "lcd.h"

void programInit() {
    buzzerInit();

    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(columnPorts[i], columnPins[i], GPIO_PIN_SET);
}
