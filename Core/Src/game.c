//
// Created by Sina on 6/30/2022.
//


#include "game.h"
#include "characters.h"
#include "stdbool.h"
#include "lcd.h"

doodlerMoveModeType doodlerMoveMode = DESCENDING;
uint32_t shiftUpCount = 7;

bool monsterCollision() {
    if (lcdArr[characters[0].x][characters[0].y] == MONSTER)
        return true;
    else if (lcdArr[characters[1].x][characters[1].y] == MONSTER)
        return true;
    return false;
}

bool holeCollision() {
    if (lcdArr[characters[0].x][characters[0].y] == HOLE)
        return true;
    else if (lcdArr[characters[1].x][characters[1].y] == HOLE)
        return true;
    return false;
}

bool fallCollision() {
    if (characters[1].y >= 19)
        return true;
    return false;
}

void lose() {

}

void doodlerMoveUp() {
    characters[0].y--;
    characters[1].y--;
}

void doodlerMoveDown() {
    characters[0].y++;
    characters[1].y++;
}


bool stepCollision() {
    if (lcdArr[characters[1].x][characters[1].y + 1] == NORMAL_STEP) {
        doodlerMoveMode = ASCENDING;
        shiftUpCount = 7;
        return true;
    } else if (lcdArr[characters[1].x][characters[1].y + 1] == SPRING_STEP) {
        doodlerMoveMode = ASCENDING;
        shiftUpCount = 20;
        return true;
    } else if (lcdArr[characters[1].x][characters[1].y + 1] == BROKEN_STEP) {
        for (int i = 2; i < 80; i++) {
            if (characters[i].x == characters[1].x && characters[i].y == (characters[1].y + 1) &&
                characters[i].type == BROKEN_STEP) {
                characters[i] = (Character) {AIR, characters[i].x, characters[i].y};
                return false;
            }
        }
    }
    return false;
}

void handleGame() {

    if (monsterCollision() || holeCollision() || fallCollision()) {
        lose();
        return;
    }

    if (doodlerMoveMode == ASCENDING) {
        if (characters[0].y <= 9) {
            if (shiftUpCount > 0) {
                shiftDownCharacters(1);
                shiftUpCount--;
            } else {
                doodlerMoveMode = DESCENDING;
            }

        } else {
            doodlerMoveUp();
            shiftUpCount--;
        }
    } else {
        if (!stepCollision()) {
            doodlerMoveDown();
        }

    }

    lcdUpdate();
}