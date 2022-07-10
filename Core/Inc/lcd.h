//
// Created by Sina on 6/28/2022.
//

#ifndef DOODLEJUMP_LCD_H
#define DOODLEJUMP_LCD_H

#include "characters.h"

#define LCD_D_8 GPIO_PIN_8
#define LCD_D_9 GPIO_PIN_9
#define LCD_D_10 GPIO_PIN_10
#define LCD_D_11 GPIO_PIN_11
#define LCD_D_12 GPIO_PIN_12
#define LCD_D_13 GPIO_PIN_13
#define LCD_D_14 GPIO_PIN_14

#define VERTICAL_LCD_ROWS 20
#define VERTICAL_LCD_COLUMNS 4

void lcdInit();

void lcdTest();

int lcdUpdate();

extern characterType lcdArray[VERTICAL_LCD_ROWS][VERTICAL_LCD_COLUMNS];

__STATIC_INLINE characterType *lcdArr(uint_fast8_t x, uint_fast8_t y) {
    return &lcdArray[y][x];
}

#endif //DOODLEJUMP_LCD_H
