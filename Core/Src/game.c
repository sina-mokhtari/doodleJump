//
// Created by Sina on 6/30/2022.
//


#include "game.h"
#include "characters.h"
#include "stdbool.h"
#include "lcd.h"
#include "buzzer.h"

#define JUMP_LITTLE 0
#define JUMP_BIG 1

uint32_t score = 0;
character *lastStep;

doodlerMoveModeType doodlerMoveMode = DESCENDING;
uint32_t shiftUpCount = 0;

bool monsterCollision() {
    if (lcdArr[doodler[0].x][doodler[0].y] == Monster ||
        lcdArr[doodler[1].x][doodler[1].y] == Monster)
        return true;
    else
        return false;
}

bool holeCollision() {
    if (lcdArr[doodler[0].x][doodler[0].y] == BlackHole ||
        lcdArr[doodler[1].x][doodler[1].y] == BlackHole)
        return true;
    else
        return false;
}

bool fallCollision() {
    if (doodler[1].y >= 19)
        return true;
    return false;
}

void lose() {

}

void doodlerMoveUp() {
    doodler[0].y--;
    doodler[1].y--;
}

void doodlerMoveDown() {
    doodler[0].y++;
    doodler[1].y++;
}

void doodlerMoveLeft() {
    if (doodler[0].x > 0 || doodler[1].x > 0) {
        doodler[0].x--;
        doodler[1].x--;
    } else {
        doodler[0].x = 3;
        doodler[1].x = 3;
    }
}

void doodlerMoveRight() {
    if (doodler[0].x < 3 || doodler[1].x < 3) {
        doodler[0].x++;
        doodler[1].x++;
    } else {
        doodler[0].x = 0;
        doodler[1].x = 0;
    }
}

character *findCharacter(uint_fast8_t x, uint_fast8_t y) {
    for (int i = 0; i < 80; i++) {
        if (characters[i].y == y &&
            characters[i].x == x) {
            return &characters[i];
        }
    }
    return NULL;
}

character *characterTmp;
melodyName melodyToPlay;

void doodlerJump(int n, uint_fast8_t stepX, uint_fast8_t stepY) {
    doodlerMoveMode = ASCENDING;
    shiftUpCount = n - 1;
    doodlerMoveUp();
    characterTmp = findCharacter(stepX, stepY);
    if (characterTmp != lastStep) {
        addScore();
        if (n == 7)
            melodyToPlay = JumpLittle;
        else
            melodyToPlay = JumpBig;

        osMessageQueuePut(melodyNameQuHandle, &melodyToPlay, 0U, 10);
        lastStep = characterTmp;
        return;
    }
}


bool doodlerReachedMiddle() {
    return doodler[0].y <= 9;
}

uint32_t getScore() {
    return score;
}

void addScore() {
    score += difficulty + 1;
}

void doodlerSwapCheck() {
    if (doodler[0].x < 0 || doodler[1].x < 0) {
        doodler[0].x = 3;
        doodler[1].x = 3;
    } else if (doodler[0].x > 3 || doodler[1].x > 3) {
        doodler[0].x = 0;
        doodler[1].x = 0;
    }
}


bool stepCollision() {
    if (lcdArr[doodler[1].y + 1][doodler[1].x] == NormalStep) {
        doodlerJump(7, doodler[1].x, doodler[1].y + 1);
        return true;
    } else if (lcdArr[doodler[1].y + 1][doodler[1].x] == SpringStep) {
        doodlerJump(20, doodler[1].x, doodler[1].y + 1);
        return true;
    } else if (lcdArr[doodler[1].y + 1][doodler[1].x] == BrokenStep) {
        for (int i = 0; i < 80; i++) {
            if (characters[i].x == doodler[1].x && characters[i].y == (doodler[1].y + 1) &&
                characters[i].type == BrokenStep) {
                characters[i] = (character) {Air, characters[i].x, characters[i].y};
                return false;
            }
        }
    }
    return false;
}


void gameStart() {
   // buzzerMelodyIntro();
}

int handleGame() {

    if (monsterCollision() || holeCollision() || fallCollision()) {
        lose();
        return -1;
    }

    //doodlerSwapCheck();

    if (doodlerMoveMode == ASCENDING) {
        if (shiftUpCount > 0) {
            if (doodlerReachedMiddle())
                shiftDownCharacters(1);
            else
                doodlerMoveUp();
            shiftUpCount--;
        } else
            doodlerMoveMode = DESCENDING;

    } else if (!stepCollision())
        doodlerMoveDown();


  return lcdUpdate();
}