//
// Created by Sina on 6/27/2022.
//
#include "requisite.h"
#include "adc.h"
#include "lcd.h"
#include "keypad.h"

void programInit() {

    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(Column_ports[i], Column_pins[i], GPIO_PIN_SET);
}