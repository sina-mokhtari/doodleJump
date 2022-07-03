//
// Created by Sina on 6/30/2022.
//

#include "requisite.h"
#include "game.h"
#include "characters.h"
#include "lcd.h"
#include "buzzer.h"

uint32_t score = 0;

doodlerMoveModeType doodlerMoveMode = Descending;
uint32_t shiftUpCount = 0;

bool monsterCollision() {
    return *lcdArr(doodler.upper.x, doodler.upper.y) == Monster ||
           *lcdArr(doodler.lower.x, doodler.lower.y) == Monster;
}

bool blackHoleCollision() {
    return *lcdArr(doodler.upper.x, doodler.lower.y) == BlackHole ||
           *lcdArr(doodler.lower.x, doodler.lower.y) == BlackHole;
}

bool fallDetect() {
    return doodler.lower.y >= VERTICAL_LCD_ROWS - 1;
}

void lose() {
    buzzerChangeTone(1000, 1000);
}

void doodlerMoveUp() {
    doodler.upper.y--;
    doodler.lower.y--;
}

void doodlerMoveDown() {
    doodler.upper.y++;
    doodler.lower.y++;
}

void doodlerMoveLeft() {
    if (doodler.upper.x > 0 || doodler.lower.x > 0) {
        doodler.upper.x--;
        doodler.lower.x--;
    } else {
        doodler.upper.x = VERTICAL_LCD_COLUMNS - 1;
        doodler.lower.x = VERTICAL_LCD_COLUMNS - 1;
    }
}

void doodlerMoveRight() {
    if (doodler.upper.x < VERTICAL_LCD_COLUMNS - 1 || doodler.lower.x < VERTICAL_LCD_COLUMNS - 1) {
        doodler.upper.x++;
        doodler.lower.x++;
    } else {
        doodler.upper.x = 0;
        doodler.lower.x = 0;
    }
}


character *characterTmp;
melodyName melodyToPlay;

void doodlerJump(uint32_t jumpHeight, uint_fast8_t stepX, uint_fast8_t stepY) {
    doodlerMoveMode = Ascending;
    shiftUpCount = jumpHeight - 1;
    doodlerMoveUp();
    characterTmp = findCharacter(stepX, stepY);
    if (!characterTmp->visited) {
        addScore();
        if (jumpHeight == 7)
            melodyToPlay = JumpLittle;
        else
            melodyToPlay = JumpBig;

        osMessageQueuePut(melodyNameQuHandle, &melodyToPlay, 0U, 10);
        characterTmp->visited = true;
        return;
    }
}


bool doodlerReachedMiddle() {
    return doodler.upper.y <= 9;
}

uint32_t getScore() {
    return score;
}

void addScore() {
    score += difficulty + 1;
}

bool stepHandle(characterType stepType) {
    switch (stepType) {
        case NormalStep:
            doodlerJump(NORMAL_JUMP_HEIGHT, doodler.lower.x, doodler.lower.y + 1);
            return true;
        case SpringStep:
            doodlerJump(HIGH_JUMP_HEIGHT, doodler.lower.x, doodler.lower.y + 1);
            return true;
        case BrokenStep: {
            character *tmpChar = findCharacter(doodler.lower.x, doodler.lower.y + 1);
            if ((*tmpChar).type == BrokenStep)
                (*tmpChar).type = Air;
            return false;
        }
        default:
            return false;
    }
}


void gameStart() {
    //buzzerMelodyIntro();
}

characterType stepCollision() {
    if (*lcdArr(doodler.lower.x, doodler.lower.y + 1) == NormalStep)
        return NormalStep;
    else if (*lcdArr(doodler.lower.x, doodler.lower.y + 1) == SpringStep)
        return SpringStep;
    else if (*lcdArr(doodler.lower.x, doodler.lower.y) == BrokenStep)
        return BrokenStep;

    return (characterType) NULL;
}

bool loseSituation() {
    return monsterCollision() || blackHoleCollision() || fallDetect();
}

void doodlerMove(doodlerMoveDirectionType direction) {
    switch (direction) {
        case Up:
            doodlerMoveUp();
            break;
        case Down:
            doodlerMoveDown();
            break;
        case Left:
            doodlerMoveLeft();
            break;
        case Right:
            doodlerMoveRight();
            break;
        default:
            break;
    }
}

void doodlerMoveHandle() {
    if (doodlerMoveMode == Ascending) {
        if (shiftUpCount > 0) {
            if (doodlerReachedMiddle())
                shiftDownCharacters(1);
            else
                doodlerMove(Up);
            shiftUpCount--;
        } else
            doodlerMoveMode = Descending;

    } else if (!stepHandle(stepCollision()))
        doodlerMove(Down);
}

int gameHandle() {

    if (loseSituation()) {
        lose();
        return -1;
    } else
        doodlerMoveHandle();

    return lcdUpdate();
}