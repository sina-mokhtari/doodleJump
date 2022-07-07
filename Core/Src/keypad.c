//
// Created by Sina on 6/27/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include "requisite.h"
#include "game.h"
#include "lcd.h"

GPIO_TypeDef *const rowPorts[] = {GPIOD, GPIOD, GPIOD, GPIOD}; // interrupts
const uint16_t rowPins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3}; // interrupts
GPIO_TypeDef *const columnPorts[] = {GPIOD, GPIOD, GPIOD, GPIOD};
const uint16_t columnPins[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};

uint32_t keypadNum;

//   C0   C1   C2   C3
// +----+----+----+----+
// | 1  | 2  | 3  | 4  |  R0
// +----+----+----+----+
// | 5  | 6  | 7  | 8  |  R1
// +----+----+----+----+
// | 9  | 10 | 11 | 12 |  R2
// +----+----+----+----+
// | 13 | 14 | 15 | 16 |  R3
// +----+----+----+----+
void keypadAssign(uint16_t gpioPin) {

    int rowNumber = -1;
    int columnNumber = -1;

#pragma unroll
    for (int row = 0; row < 4; row++) // Loop through Rows
        if (gpioPin == rowPins[row]) {
            rowNumber = row;
            break;
        }

#pragma unroll
    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(columnPorts[i], columnPins[i], GPIO_PIN_RESET);

#pragma unroll
    for (int col = 0; col < 4; col++) {     // Loop through Columns
        HAL_GPIO_WritePin(columnPorts[col], columnPins[col], GPIO_PIN_SET);
        if (HAL_GPIO_ReadPin(rowPorts[rowNumber], rowPins[rowNumber])) {
            columnNumber = col;
            break;
        }
        //HAL_GPIO_WritePin(columnPorts[col], columnPins[col], GPIO_PIN_RESET); //  not sure if commenting this is ok
    }

#pragma unroll
    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(columnPorts[i], columnPins[i], GPIO_PIN_SET);

    if (rowNumber == -1 || columnNumber == -1)
        return; // Reject invalid scan

    const uint8_t buttonNumber = rowNumber * 4 + columnNumber + 1;

    keypadNum = buttonNumber;
}

bool firstTime = true;

void keypadHandle() {
    switch (keypadNum) {
        case 1:
            if (firstTime) {
                srandom(osKernelGetTickCount());
                lcdInit();
                firstTime = false;
            }
            gameStat = Playing;

            if (osThreadGetState(updateLcdTskHandle) == osThreadBlocked) {
                osThreadResume(updateLcdTskHandle);
                gameStart();
            }
            break;
        case 13:
            doodlerMove(Left);
            break;
        case 14:
            doodlerGunFire();
            break;
        case 15:
            doodlerMove(Right);
            break;
        default:
            break;
    }

}
