//
// Created by Sina on 7/9/2022.
//
#include <stdio.h>
#include <stdlib.h>
#include "requisite.h"
#include "LiquidCrystal.h"
#include "timeManagement.h"
#include "game.h"
#include "menu.h"

char groupName[25] = "Default Name";
static char timeStr[20];
static char scoreStr[10];
static char difficultyStr[2];

void aboutStart() {

    updateDateTime();
    sprintf(timeStr, "%d/%d/%d - %d:%d:%d",
            getDateTime().dateVar.Year, getDateTime().dateVar.Month, getDateTime().dateVar.Date,
            getDateTime().timeVar.Hours, getDateTime().timeVar.Minutes, getDateTime().timeVar.Seconds);

    itoa((int) score, scoreStr, 10);
    itoa((int) difficulty, difficultyStr, 10);

    osMutexAcquire(lcdMutexHandle, osWaitForever);

    clear();

    setCursor(0, 0);
    print("Name: ");
    print(groupName);

    setCursor(0, 1);
    print("Last Score: ");
    print(scoreStr);

    setCursor(0, 2);
    print("Difficulty: ");
    print(difficultyStr);

    setCursor(0, 3);
    print(timeStr);
    osMutexRelease(lcdMutexHandle);
}

void aboutHandle() {
    updateDateTime();
    sprintf(timeStr, "%d/%d/%d - %d:%d:%d",
            getDateTime().dateVar.Year, getDateTime().dateVar.Month, getDateTime().dateVar.Date,
            getDateTime().timeVar.Hours, getDateTime().timeVar.Minutes, getDateTime().timeVar.Seconds);

    osMutexAcquire(lcdMutexHandle, osWaitForever);
    setCursor(0, 3);
    print(timeStr);
    osMutexRelease(lcdMutexHandle);
}

void aboutKeypadHandle() {
    switch (keypadNum) {
        case 0:
            programState = MenuState;
            menuStart();
            break;
        default:
            break;
    }
}