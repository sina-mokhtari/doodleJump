//
// Created by Sina on 6/28/2022.
//

#include <stdio.h>
#include "requisite.h"
#include "LiquidCrystal.h"
#include "lcd.h"
#include "characters.h"


characterType lcdArray[VERTICAL_LCD_ROWS][VERTICAL_LCD_COLUMNS] = {Air};


void lcdInit() {
    liquidCrystal(GPIOD, LCD_D_8, LCD_D_9, LCD_D_10, LCD_D_11, LCD_D_12, LCD_D_13,
                  LCD_D_14);
    begin(VERTICAL_LCD_ROWS, VERTICAL_LCD_COLUMNS);
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

bool collisionWithDoodler(uint32_t x, uint32_t y) {
    return (x == doodler.upper.x && y == doodler.upper.y) ||
           (x == doodler.lower.x && y == doodler.lower.y);
}

bool isBadCharacter(characterType inputCharacter) {
    return inputCharacter == Monster ||
           inputCharacter == BlackHole;
}

character tmpCharacter;

int lcdWriteCount = 0;
bool foundBadCharacter;

int lcdUpdate() {
    lcdWriteCount = 2;

    // refreshing lcd
    foundBadCharacter = false;
    for (int i = 0; i < CHARACTERS_NUMBER; i++) {
        tmpCharacter = *getCharacter(i);
        if (*lcdArr(tmpCharacter.x, tmpCharacter.y) != tmpCharacter.type) {
            if (collisionWithDoodler(tmpCharacter.x, tmpCharacter.y)) {
//                if (isBadCharacter(*lcdArr(tmpCharacter.y][tmpCharacter.x])) {
//                    foundBadCharacter = true;
//                    *lcdArr(tmpCharacter.x,tmpCharacter.y) = tmpCharacter.type;
//                    break;
//                  }

            } else {
                *lcdArr(tmpCharacter.x, tmpCharacter.y) = tmpCharacter.type;
                setCursor(tmpCharacter.y, 3 - tmpCharacter.x);
                write(tmpCharacter.type);
                lcdWriteCount++;
                //if (foundBadCharacter)
                // break;
            }
        }
    }
    if (!foundBadCharacter) {
        tmpCharacter = doodler.upper;
        *lcdArr(tmpCharacter.x, tmpCharacter.y) = tmpCharacter.type;
        setCursor(tmpCharacter.y, 3 - tmpCharacter.x);
        write(tmpCharacter.type);

        tmpCharacter = doodler.lower;
        *lcdArr(tmpCharacter.x, tmpCharacter.y) = tmpCharacter.type;
        setCursor(tmpCharacter.y, 3 - tmpCharacter.x);
        write(tmpCharacter.type);
    }

    return lcdWriteCount;
}
