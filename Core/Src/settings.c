//
// Created by Sina on 7/10/2022.
//
#include "requisite.h"
#include "LiquidCrystal.h"
#include "about.h"
#include "game.h"
#include "menu.h"
#include "acceleroGyro.h"
#include "uart.h"

typedef enum {
    ChangeNameOption,
    AccelerometerControl,
    LoadGame
} settingsOptionType;


static settingsOptionType optionsArr[3] = {ChangeNameOption, AccelerometerControl, LoadGame};
static int currentOptionIdx = 0;
static int newOptionIdx = 0;
static bool selectKeyPressed = false;
static bool keyboardEnabled = false;

bool gameLoaded = false;
void settingsStart() {
    osMutexAcquire(lcdMutexHandle, osWaitForever);

    clear();
    setCursor(1, 0);
    print("Change Name");

    setCursor(1, 1);
    if (accelerometerControlEnabled)
        print("Accelerometer On");
    else
        print("Accelerometer Off");

    setCursor(0, currentOptionIdx);
    print("*");

    setCursor(1, 2);
    print("Load Game");
    osMutexRelease(lcdMutexHandle);
}

static const char abc[4] = {'2', 'a', 'b', 'c'}, def[4] = {'3', 'd', 'e', 'f'},
        ghi[4] = {'4', 'g', 'h', 'i'}, jkl[4] = {'5', 'j', 'k', 'l'},
        mno[4] = {'6', 'm', 'n', 'o'}, pqrs[5] = {'7', 'p', 'q', 'r', 's'},
        tuv[4] = {'8', 't', 'u', 'v'}, wxyz[5] = {'9', 'w', 'x', 'y', 'z'};

void keyboardHandle() {
    static int charIdx = 0;
    static int writeIdx = 0;
    static char charToWrite;
    static bool selected = false;
    switch (keypadNum) {
        case 0:
            groupName[writeIdx] = '\0';
            writeIdx = 0;
            charIdx = 0;
            selected = false;
            keyboardEnabled = false;
            settingsStart();
            break;
        case 1:
            charToWrite = '1';
            break;
        case 2:
            charToWrite = abc[charIdx];
            charIdx = (charIdx + 1) % 4;
            break;
        case 3:
            charToWrite = def[charIdx];
            charIdx = (charIdx + 1) % 4;
            break;
        case 5:
            charToWrite = ghi[charIdx];
            charIdx = (charIdx + 1) % 4;
            break;
        case 6:
            charToWrite = jkl[charIdx];
            charIdx = (charIdx + 1) % 4;
            break;
        case 7:
            charToWrite = mno[charIdx];
            charIdx = (charIdx + 1) % 4;
            break;
        case 9:
            charToWrite = pqrs[charIdx];
            charIdx = (charIdx + 1) % 5;
            break;
        case 10:
            charToWrite = tuv[charIdx];
            charIdx = (charIdx + 1) % 4;
            break;
        case 11:
            charToWrite = wxyz[charIdx];
            charIdx = (charIdx + 1) % 5;
            break;
        case 13:
            charToWrite = '*';
            break;
        case 14:
            charToWrite = ' ';
            break;
        case 15:
            charToWrite = '#';
            break;
        case 8:
            selected = true;
            break;
        default:
            break;
    }
    if (keyboardEnabled) {
        groupName[writeIdx] = charToWrite;
        osMutexAcquire(lcdMutexHandle, osWaitForever);
        setCursor(writeIdx, 1);
        print(&charToWrite);
        osMutexRelease(lcdMutexHandle);
        if (selected) {
            writeIdx++;
            selected = false;
        }
    }
}

void keyboardStart() {
    osMutexAcquire(lcdMutexHandle, osWaitForever);
    clear();
    osMutexRelease(lcdMutexHandle);
}

static void select() {
    switch (optionsArr[currentOptionIdx]) {
        case ChangeNameOption:
            keyboardEnabled = true;
            keyboardStart();
            break;
        case AccelerometerControl:
            accelerometerControlEnabled = !accelerometerControlEnabled;
            osMutexAcquire(lcdMutexHandle, osWaitForever);
            setCursor(15, 1);
            if (accelerometerControlEnabled)
                print("On ");
            else
                print("Off");
            osMutexRelease(lcdMutexHandle);
            break;
        case LoadGame:
            uartFormatTransmit("2load-request");
            osDelay(6000);
            //osMutexAcquire(gameLoadMutexHandle, osWaitForever);
            //osMutexAcquire(gameLoadMutexHandle, 9000);
            osMutexAcquire(lcdMutexHandle, osWaitForever);
            setCursor(11, 2);
            if (loadDone) {
                print("Success!");
                loadDone = false;
                gameLoaded = true;
            } else {
                print("Failed!");
            }
            osDelay(2000);
            setCursor(11, 2);
            print("        ");
            osMutexRelease(lcdMutexHandle);
            //osMutexRelease(gameLoadMutexHandle);
            break;
        default:
            break;
    }
    selectKeyPressed = false;
}

void settingsKeypadHandle() {
    if (keyboardEnabled)
        keyboardHandle();
    else
        switch (keypadNum) {
            case 0:
                programState = MenuState;
                menuStart();
                break;
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


void settingsHandle() {
    if (!keyboardEnabled) {

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
}