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


character *tmpCharacter2;

int lcdWriteCount2 = 0;

int lcdUpdate() {
    lcdWriteCount2 = 2;

    for (int i = 0; i < VERTICAL_LCD_ROWS; i++) {
        for (int j = 0; j < VERTICAL_LCD_COLUMNS; j++) {
            tmpCharacter2 = getCharacter(j, i);
            if (*lcdArr(tmpCharacter2->x, tmpCharacter2->y) != tmpCharacter2->type) {
                if (!collisionWithDoodler(tmpCharacter2->x, tmpCharacter2->y)) {
                    *lcdArr(tmpCharacter2->x, tmpCharacter2->y) = tmpCharacter2->type;
                    setCursor(tmpCharacter2->y, 3 - tmpCharacter2->x);
                    write(tmpCharacter2->type);
                    lcdWriteCount2++;
                }
            }
        }
    }
    *lcdArr(doodler.upper.x, doodler.upper.y) = doodler.upper.type;
    setCursor(doodler.upper.y, 3 - doodler.upper.x);
    write(doodler.upper.type);

    *lcdArr(doodler.lower.x, doodler.lower.y) = doodler.lower.type;
     setCursor(doodler.lower.y, 3 - doodler.lower.x);
    write(doodler.lower.type);

    for (int i = 0; i < BULLETS_BUFFER_SIZE; i++) {
        if (bullets[i].characterFlag) {
            *lcdArr(bullets[i].x, bullets[i].y) = bullets[i].type;
            setCursor(bullets[i].y, 3 - bullets[i].x);
            write(bullets[i].type);
            lcdWriteCount2++;
        }
    }

    return lcdWriteCount2;
}


