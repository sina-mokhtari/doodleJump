//
// Created by Sina on 6/28/2022.
//

#ifndef DOODLEJUMP_LCD_H
#define DOODLEJUMP_LCD_H

#include "characters.h"

#define LCD_D8 GPIO_PIN_8
#define LCD_D9 GPIO_PIN_9
#define LCD_D10 GPIO_PIN_10
#define LCD_D11 GPIO_PIN_11
#define LCD_D12 GPIO_PIN_12
#define LCD_D13 GPIO_PIN_13
#define LCD_D14 GPIO_PIN_14

void lcdInit();

void lcdTest();

int lcdUpdate();


extern characterType lcdArr[20][4];
#endif //DOODLEJUMP_LCD_H
