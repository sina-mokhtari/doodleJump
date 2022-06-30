//
// Created by Sina on 6/28/2022.
//

#include "requisite.h"
#include "LiquidCrystal.h"
#include "lcd.h"
#include "stdio.h"
#include "characters.h"


char lcdStr[100];

characterType lcdArr[20][4] = {DEFAULT_CHARACTER_TYPE};

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

}

void lcdTest() {
    print("yess");
}

Character tmpCharacter;

int lcdWriteCount;

int lcdUpdate() {
    lcdWriteCount = 0;

    // refreshing lcd
    for (int i = 2; i < 82; i++) {
        tmpCharacter = characters[i];
        if (lcdArr[tmpCharacter.y][tmpCharacter.x] != tmpCharacter.type) {
            lcdArr[tmpCharacter.y][tmpCharacter.x] = tmpCharacter.type;
            setCursor(tmpCharacter.y, 3 - tmpCharacter.x);
            write(tmpCharacter.type);
            lcdWriteCount++;
        }
    }
    tmpCharacter = characters[0];
    lcdArr[tmpCharacter.y][tmpCharacter.x] = tmpCharacter.type;
    setCursor(tmpCharacter.y, 3 - tmpCharacter.x);
    write(tmpCharacter.type);

    tmpCharacter = characters[1];
    lcdArr[tmpCharacter.y][tmpCharacter.x] = tmpCharacter.type;
    setCursor(tmpCharacter.y, 3 - tmpCharacter.x);
    write(tmpCharacter.type);

    lcdWriteCount++;
    lcdWriteCount++;
    /*  setCursor(7, 0);
      sprintf(lcdStr, "%lu", score);
      print(lcdStr);*/
    return lcdWriteCount;
}
