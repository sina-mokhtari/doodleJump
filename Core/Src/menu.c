//
// Created by Sina on 7/9/2022.
//
#include "requisite.h"
#include "keypad.h"
#include "LiquidCrystal.h"
#include "game.h"
#include "about.h"
#include "settings.h"

typedef enum {
    PlayOption, AboutOption, SettingsOption
} menuOptionType;

static menuOptionType optionsArr[3] = {PlayOption, SettingsOption, AboutOption};
static int currentOptionIdx = 0;
static int newOptionIdx = 0;
static bool selectKeyPressed = false;

static void select() {
    switch (optionsArr[currentOptionIdx]) {

        case PlayOption:
            gameStart();
            programState = PlayingState;
            break;
        case SettingsOption:
            settingsStart();
            programState = SettingsState;
            break;
        case AboutOption:
            aboutStart();
            programState = AboutState;
            break;
    }
    selectKeyPressed = false;
}

void menuStart() {
    osMutexAcquire(lcdMutexHandle, osWaitForever);
    clear();
    setCursor(1, 0);
    print("Play");
    setCursor(1, 1);
    print("Settings");
    setCursor(1, 2);
    print("About");

    setCursor(0, currentOptionIdx);
    print("*");
    osMutexRelease(lcdMutexHandle);
}

void menuKeypadHandle() {
    switch (keypadNum) {
        case 2:
            newOptionIdx = currentOptionIdx > 0 ? currentOptionIdx - 1 : 2;
            break;
        case 10:
            newOptionIdx = currentOptionIdx < 2 ? currentOptionIdx + 1 : 0;
            break;
        case 6:
            selectKeyPressed = true;
            break;
        default:
            break;
    }
}

void menuHandle() {

    if (newOptionIdx != currentOptionIdx) {

        osMutexAcquire(lcdMutexHandle, osWaitForever);
        setCursor(0, currentOptionIdx);
        print(" ");

        setCursor(0, newOptionIdx);
        print("*");
        osMutexRelease(lcdMutexHandle);

        currentOptionIdx = newOptionIdx;
    }

    if (selectKeyPressed) {
        select();
    }
}