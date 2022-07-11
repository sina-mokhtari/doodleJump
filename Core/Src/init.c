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
    uartReceive(23);
    accelerometerInit();
    //gyroscopeInit();
    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(columnPorts[i], columnPins[i], GPIO_PIN_SET);
//-----------------------------------   test
   /* *getCharacter(0, 0) = (character) {SpringStep, 0, 0, true};
    *getCharacter(2, 4) = (character) {SpringStep, 2, 4, false};
    *getCharacter(2, 9) = (character) {NormalStep, 2, 9, false};
    *getCharacter(2, 13) = (character) {NormalStep, 2, 13, false};
    *getCharacter(3, 19) = (character) {NormalStep, 3, 19, false};

    difficulty = 7;
    score = 841;
    doodlerMoveMode = Descending;
    shiftUpCount = 13;*/
}

