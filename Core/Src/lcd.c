//
// Created by Sina on 6/28/2022.
//

#include "requisite.h"
#include "LiquidCrystal.h"
#include "lcd.h"
#include "stdio.h"

char lcdStr[100];

void lcdInit() {
    LiquidCrystal(GPIOD, LCD_D8, LCD_D9, LCD_D10, LCD_D11, LCD_D12, LCD_D13,
                  LCD_D14);
    begin(20, 4);
    print("score: ");
}

void lcdTest() {
    print("yess");
}

void lcdUpdate() {
    setCursor(7, 0);
    sprintf(lcdStr, "%lu", score);
    print(lcdStr);
}
