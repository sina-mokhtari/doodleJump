//
// Created by Sina on 7/10/2022.
//

#include <stdlib.h>
#include "game.h"
#include "characters.h"
#include "buzzer.h"
#include "LiquidCrystal.h"

void lcdDisplayScoreAfterLose() {
    char tmpStr[10];

    itoa((int) score, tmpStr, 10);
    osMutexAcquire(lcdMutexHandle, osWaitForever);
    clear();
    setCursor(0, 1);
    print("Score: ");
    print(tmpStr);

    itoa((int) difficulty, tmpStr, 10);
    setCursor(0, 3);
    print("Difficulty: ");
    print(tmpStr);
    osMutexRelease(lcdMutexHandle);
}

void lcdBlackHoleAnimation() {
    osMutexAcquire(lcdMutexHandle, osWaitForever);
    clear();
    setCursor(9, 3);
    write(0xff);
    write(0xff);
    write(0xff);

    setCursor(9, 0);
    write(0xff);
    write(0xff);
    write(0xff);

    setCursor(8, 2);
    write(0xff);
    write(0xff);
    write(0xff);
    write(0xff);
    write(0xff);

    setCursor(8, 1);
    write(0xff);
    write(0xff);
    write(0xff);
    write(0xff);
    write(0xff);

    setCursor(1, 3);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(1, 3);
    print("  ");

    setCursor(1, 0);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(1, 0);
    print("  ");

    setCursor(18, 0);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(18, 0);
    print("  ");

    setCursor(18, 3);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(18, 3);
    print("  ");

    setCursor(3, 3);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(3, 3);
    print("  ");

    setCursor(3, 0);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(3, 0);
    print("  ");

    setCursor(16, 0);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(16, 0);
    print("  ");

    setCursor(16, 3);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(16, 3);
    print("  ");

    setCursor(5, 3);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(5, 3);
    print("  ");

    setCursor(5, 0);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(5, 0);
    print("  ");

    setCursor(14, 0);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(14, 0);
    print("  ");

    setCursor(14, 3);
    write(DoodlerUp);
    write(DoodlerDown);
    osDelay(200);
    setCursor(14, 3);
    print("  ");
    osMutexRelease(lcdMutexHandle);
    osDelay(500);

    lcdDisplayScoreAfterLose();
}


void lcdFallAnimation() {
    osMutexAcquire(lcdMutexHandle, osWaitForever);
    clear();
    setCursor(0, 3 - doodler.upper.x);
    write(doodler.upper.type);
    setCursor(1, 3 - doodler.lower.x);
    write(doodler.lower.type);

    for (int i = 0; i < 18; ++i) {
        scrollDisplayRight();
        osDelay(200);
    }
    osMutexRelease(lcdMutexHandle);
}

void fall() {
    programState = LoseState;
    melodyQueueSend(MelodyFall);
    lcdFallAnimation();
}

void blackHole() {
    programState = LoseState;
    melodyQueueSend(MelodyBlackHole);
    lcdBlackHoleAnimation();
}

void loseKeypadHandle() {
    if (keypadNum == 0) {
        osMutexAcquire(lcdMutexHandle, osWaitForever);
        clear();
        osMutexRelease(lcdMutexHandle);
        programState = IntroState;
    }
}