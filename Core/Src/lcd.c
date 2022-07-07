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

void lcdLose() {

    clear();

    setCursor(0, 3 - doodler.upper.x);
    write(doodler.upper.type);
    setCursor(1, 3 - doodler.lower.x);
    write(doodler.lower.type);

    for (int i = 0; i < 18; ++i) {
        scrollDisplayRight();
        osDelay(200);
    }

    clear();
}

void lcdTest() {

}

character tmpCharacter;

int lcdWriteCount = 0;

int lcdUpdate() {
    lcdWriteCount = 2;


    for (int i = 0; i < CHARACTERS_NUMBER; i++) {
        tmpCharacter = *getCharacter(i);
        if (*lcdArr(tmpCharacter.x, tmpCharacter.y) != tmpCharacter.type) {
            if (!collisionWithDoodler(tmpCharacter.x, tmpCharacter.y)) {
                *lcdArr(tmpCharacter.x, tmpCharacter.y) = tmpCharacter.type;
                setCursor(tmpCharacter.y, 3 - tmpCharacter.x);
                write(tmpCharacter.type);
                lcdWriteCount++;
            }
        }
    }

    *lcdArr(doodler.upper.x, doodler.upper.y) = doodler.upper.type;
    setCursor(doodler.upper.y, 3 - doodler.upper.x);
    write(doodler.upper.type);

    *lcdArr(doodler.lower.x, doodler.lower.y) = doodler.lower.type;
    // setCursor(doodler.lower.y, 3 - doodler.lower.x);
    write(doodler.lower.type);

    for (int i = 0; i < BULLETS_BUFFER_SIZE; i++) {
        if (bullets[i].characterFlag) {
            *lcdArr(bullets[i].x, bullets[i].y) = bullets[i].type;
            setCursor(bullets[i].y, 3 - bullets[i].x);
            write(bullets[i].type);
        }
    }

    return lcdWriteCount;
}
