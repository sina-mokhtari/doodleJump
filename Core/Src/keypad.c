//
// Created by Sina on 6/27/2022.
//
#include "requisite.h"

GPIO_TypeDef *const Row_ports[] = {GPIOD, GPIOD, GPIOD, GPIOD};
const uint16_t Row_pins[] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3}; // interrupt
GPIO_TypeDef *const Column_ports[] = {GPIOD, GPIOD, GPIOD, GPIOD};
const uint16_t Column_pins[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7};


void handleKeypad(uint16_t GPIO_Pin) {

    int row_number = -1;
    int column_number = -1;

    for (int row = 0; row < 4; row++) // Loop through Rows
    {
        if (GPIO_Pin == Row_pins[row]) {
            row_number = row;
        }
    }

    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(Column_ports[i], Column_pins[i], GPIO_PIN_RESET);

    for (int col = 0; col < 4; col++) // Loop through Columns
    {
        HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 1);
        if (HAL_GPIO_ReadPin(Row_ports[row_number], Row_pins[row_number])) {
            column_number = col;
        }
        HAL_GPIO_WritePin(Column_ports[col], Column_pins[col], 0);
    }

    for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(Column_ports[i], Column_pins[i], GPIO_PIN_SET);

    if (row_number == -1 || column_number == -1) {
        return; // Reject invalid scan
    }
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
    const uint8_t button_number = row_number * 4 + column_number + 1;
    keypadNum = button_number;
}
