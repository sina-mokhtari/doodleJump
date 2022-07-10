//
// Created by Sina on 6/27/2022.
//
#include "requisite.h"
#include "keypad.h"
#include "buzzer.h"
#include "game.h"
#include "uart.h"
#include "acceleroGyro.h"
#include "lcd.h"

uint8_t okTest = 3;

void programInit() {
    programState = IntroState;
    buzzerInit();
    uartReceive(24);
    accelerometerInit();
    //gyroscopeInit();
    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(columnPorts[i], columnPins[i], GPIO_PIN_SET);
}
