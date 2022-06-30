//
// Created by Sina on 6/28/2022.
//

#include "requisite.h"
#include "LiquidCrystal.h"
#include "lcd.h"
#include "stdio.h"
#include "characters.h"


char lcdStr[100];

characterType lcdArr[20][4];

void lcdInit() {
    LiquidCrystal(GPIOD, LCD_D8, LCD_D9, LCD_D10, LCD_D11, LCD_D12, LCD_D13,
                  LCD_D14);
    begin(20, 4);
    createChar(0, doodlerUpByte);
    createChar(1, doodlerDownByte);
    createChar(2, normalStepByte);
    createChar(3, brokenStepByte);
    createChar(4, springStepByte);
    createChar(5, monsterByte);
    createChar(6, holeByte);
    //setCursor(3, 1);
    //write(0xef);
    //write(0);
//    write(DOODLER_UP);
//    setCursor(4, 1);
//    write(DOODLER_DOWN);
//    setCursor(5, 1);
//    write(BROKEN_STEP);
//    setCursor(0, 3);
//    write(MONSTER);
//    setCursor(2, 2);
//    write(BULLET);
}

void lcdTest() {
    print("yess");
}

Character tmpCharacter;

void lcdInitFirst() {
    for (int i = 0; i < 80; i++) {
        tmpCharacter = characters[i];
        lcdArr[tmpCharacter.x][tmpCharacter.y] = tmpCharacter.type;
        setCursor( tmpCharacter.y,3 - tmpCharacter.x);
        write(tmpCharacter.type);

    }
}

void lcdUpdate() {

    // updating characters arr

    // refreshing lcd
    for (int i = 0; i < 80; i++) {
        tmpCharacter = characters[i];
        if (lcdArr[tmpCharacter.x][tmpCharacter.y] != tmpCharacter.type) {
            lcdArr[tmpCharacter.x][tmpCharacter.y] = tmpCharacter.type;
            setCursor( tmpCharacter.y,3 - tmpCharacter.x);
            write(tmpCharacter.type);
        }
    }
    /*  setCursor(7, 0);
      sprintf(lcdStr, "%lu", score);
      print(lcdStr);*/
}
