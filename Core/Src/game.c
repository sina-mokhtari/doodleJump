//
// Created by Sina on 6/30/2022.
//

#include "requisite.h"
#include "game.h"
#include "characters.h"
#include "lcd.h"
#include "buzzer.h"
#include "uart.h"
#include "LiquidCrystal.h"
#include "timeManagement.h"

uint32_t score = 0;

doodlerMoveModeType doodlerMoveMode;
uint32_t shiftUpCount = 0;


gameStateType gameStat;

melodyName melodyToPlay;


bool isMonster(character *suspect) {
    if (suspect->type == Monster) {
        uartFormatTransmit("lose by monster\n");
        return true;
    }
    return false;
}

bool isBlackHole(character *suspect) {
    if (suspect->type == BlackHole) {
        uartFormatTransmit("lose by black hole\n");
        return true;
    }
    return false;
}

bool fallDetect() {
    if (doodler.lower.y >= VERTICAL_LCD_ROWS - 1) {
        uartFormatTransmit("lose by fall\n");
        return true;
    }
    return false;
}

void lose() {
    gameStat = Losing;
    melodyToPlay = Lose;
    osMessageQueuePut(melodyNameQuHandle, &melodyToPlay, 0U, 10);
    lcdLose();
    gameStat = IntroState; // temporary
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

void doodlerJump(uint32_t jumpHeight, uint_fast8_t stepX, uint_fast8_t stepY) {
    doodlerMoveMode = Ascending;
    shiftUpCount = jumpHeight - 1;
    doodlerMoveUp();
    characterTmp = getCharacter(stepX, stepY);
    if (!characterTmp->characterFlag) {
        addScore();
        if (jumpHeight == 7)
            melodyToPlay = JumpLittle;
        else
            melodyToPlay = JumpBig;

        osMessageQueuePut(melodyNameQuHandle, &melodyToPlay, 0U, 10);
        characterTmp->characterFlag = true;
        return;
    }
}


bool doodlerReachedMiddle() {
    return doodler.upper.y <= 9;
}

uint32_t getScore() {
    return score;
}

void resetScore() {
    score = 0;
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
            character *tmpChar = getCharacter(doodler.lower.x, doodler.lower.y + 1);
            if (tmpChar->type == BrokenStep)
                tmpChar->type = Air;
            return false;
        }
        default:
            return false;
    }
}


void gameStart() {
    clear();

    charactersInit();

    lcdUpdate();

    doodlerMoveMode = Descending;

    dateTime.timeVar.Hours = 0;
    dateTime.timeVar.Minutes = 0;
    dateTime.timeVar.Seconds = 0;

    dateTime.dateVar.Year = 0;
    dateTime.dateVar.Month = 0;
    dateTime.dateVar.Date = 0;

    HAL_RTC_SetTime(&hrtc, &dateTime.timeVar, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &dateTime.dateVar, RTC_FORMAT_BIN);

    gameStat = Playing;
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

    return isMonster(getCharacter(doodler.upper.x, doodler.upper.y)) ||
           isBlackHole(getCharacter(doodler.upper.x, doodler.upper.y)) ||
           fallDetect() ||
           isMonster(getCharacter(doodler.lower.x, doodler.lower.y)) ||
           isBlackHole(getCharacter(doodler.lower.x, doodler.lower.y));
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

void doodlerGunFire() {
    for (int i = 0; i < BULLETS_BUFFER_SIZE; i++) {
        if (!bullets[i].characterFlag) {
            bullets[i] = (character) {Bullet, doodler.upper.x, doodler.upper.y - 1, true};
            break;
        }
    }
}

void bulletHandle() {
    for (int i = 0; i < BULLETS_BUFFER_SIZE; i++) {
        if (bullets[i].characterFlag) {
            if (isMonster(getCharacter(bullets[i].x, bullets[i].y))) {
                *getCharacter(bullets[i].x, bullets[i].y) =
                        (character) {Air, bullets[i].x, bullets[i].y, false};
                bullets[i].characterFlag = false;
                continue;
            } else if (bullets[i].y > 0) {
                if (!doodlerReachedMiddle())
                    bullets[i].y--;
            } else
                bullets[i].characterFlag = false;
        }
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
    switch (gameStat) {
        case IntroState:
            break;
        case Menu:
            break;
        case Playing:

            if (loseSituation()) {
                lose();
                return 0;
            }

            bulletHandle();
            doodlerMoveHandle();

            return lcdUpdate();

            break;
        case Losing:
            osThreadSuspend(updateLcdTskHandle);
            break;
        default:
            break;
    }
    return 0;
}