//
// Created by Sina on 6/30/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include "requisite.h"
#include "game.h"
#include "characters.h"
#include "lcd.h"
#include "buzzer.h"
#include "uart.h"
#include "LiquidCrystal.h"
#include "timeManagement.h"
#include "menu.h"
#include "intro.h"
#include "about.h"
#include "settings.h"
#include "lose.h"

uint32_t score = 0;

uint32_t difficulty = 2;

doodlerMoveModeType doodlerMoveMode;

uint32_t shiftUpCount = 0;

programStateType programState;


bool isMonster(character *suspect) {
    if (suspect->type == Monster) {
        // uartFormatTransmit("fall by monster\n");
        return true;
    }
    return false;
}

bool isBlackHole(character *suspect) {
    if (suspect->type == BlackHole) {
        // uartFormatTransmit("fall by black hole\n");
        return true;
    }
    return false;
}

bool fallDetect() {
    if (doodler.lower.y >= VERTICAL_LCD_ROWS - 1) {
        //   uartFormatTransmit("fall by fall\n");
        return true;
    }
    return false;
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
            melodyQueueSend(MelodyJumpLittle);
        else
            melodyQueueSend(MelodyJumpBig);

        characterTmp->characterFlag = true;
        return;
    }
}


bool doodlerReachedMiddle() {
    return doodler.upper.y <= 9;
}

void resetScore() {
    score = 0;
}

void addScore() {
    score += difficulty + 1;
}

bool stepHandle(characterType stepType) {
    if (doodler.dizzy)
        return false;

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

bool firstTime = true;

void gameStart() {
    if (firstTime) {
        srandom(osKernelGetTickCount());
        firstTime = false;
    }

    resetScore();

    gameCharactersInit();

    osMutexAcquire(lcdMutexHandle, osWaitForever);
    clear();
    osMutexRelease(lcdMutexHandle);
    lcdUpdate();

    doodlerMoveMode = Descending;

    /*dateTime.timeVar.Hours = 0;
    dateTime.timeVar.Minutes = 0;
    dateTime.timeVar.Seconds = 0;

    dateTime.dateVar.Year = 0;
    dateTime.dateVar.Month = 0;
    dateTime.dateVar.Date = 0;

    HAL_RTC_SetTime(&hrtc, &dateTime.timeVar, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &dateTime.dateVar, RTC_FORMAT_BIN);*/
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
    if (isMonster(getCharacter(doodler.upper.x, doodler.upper.y)) ||
        isMonster(getCharacter(doodler.lower.x, doodler.lower.y))) {
        doodlerMoveMode = Descending;
        shiftUpCount = 0;
        doodler.dizzy = true;
        doodler.upper.type = DoodlerUpDizzy;
    }
    if (isBlackHole(getCharacter(doodler.upper.x, doodler.upper.y)) ||
        isBlackHole(getCharacter(doodler.lower.x, doodler.lower.y))) {
        blackHole();
        return true;
    }

    if (fallDetect()) {
        fall();
        return true;
    }

    return false;
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

void playingStateKeypadHandle() {
    switch (keypadNum) {

        case 13:
            doodlerMove(Left);
            break;
        case 14:
            doodlerGunFire();
            break;
        case 15:
            doodlerMove(Right);
            break;
        default:
            break;
    }
}

int gameHandle() {
    if (loseSituation()) {
        return 0;
    }

    bulletHandle();

    doodlerMoveHandle();

    return lcdUpdate();
}

int programRun() {
    switch (programState) {
        case IntroState:
            introHandle();
            break;
        case MenuState:
            menuHandle();
            break;
        case PlayingState:
            gameHandle();
        case LoseState:
            // just wait for blue button interrupt
            break;
        case AboutState:
            aboutHandle();
            break;
        case SettingsState:
            settingsHandle();
            break;
        default:
            break;
    }
    return 0;
}