//
// Created by Sina on 7/4/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "requisite.h"
#include "globals.h"
#include "game.h"
#include "lcd.h"
#include "characters.h"
#include "timeManagement.h"
#include "acceleroGyro.h"

#define GAME_SYNC_SIGNAL_NUMBER '0'
#define GAME_SAVE_SIGNAL_NUMBER '1'

#define GAME_SYNC_MESSAGE_LENGTH 166
#define GAME_SAVE_MESSAGE_LENGTH 1000

typedef enum {
    ControlLeft, ControlRight, ControlFire, ClockSync, LoadApprove, Loading
} uartSignalType;


char uartTmpStr[100];
char uartRxTmpStr[450];


void uartFormatTransmit(const char *format, ...) {
    int len;
    va_list arguments;

    va_start(arguments, format);
    len = vsnprintf(uartTmpStr, strlen(format) + 1, format, arguments);
    va_end(arguments);

    osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);

    HAL_UART_Transmit_DMA(&huart3, (uint8_t *) uartTmpStr, len);
}

void uartStringTransmit(const char *string) {

    osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);

    HAL_UART_Transmit_DMA(&huart3, (uint8_t *) string, strlen(string));
}

void uartReceive(uint16_t size) {
    HAL_UART_Receive_DMA(&huart3, (uint8_t *) uartRxTmpStr, size);
}

uartSignalType signalHash(const char *string) {
    if (strncmp(uartRxTmpStr, "control-", 8) == 0) {

        if (strncmp(uartRxTmpStr + 8, "left", 4) == 0)
            return ControlLeft;

        else if (strncmp(uartRxTmpStr + 8, "right", 5) == 0)
            return ControlRight;

        else if (strncmp(uartRxTmpStr + 8, "fire", 4) == 0)
            return ControlFire;

    } else if (strncmp(uartRxTmpStr, "clock-", 6) == 0) {
        return ClockSync;
    } else if (strncmp(uartRxTmpStr, "load-approve", 12) == 0) {  //  load-approve------------
        return LoadApprove;
    } else if (strncmp(uartRxTmpStr, "loading-", 8) == 0) {  //
        return Loading;
    }
    return (uartSignalType) NULL;
}

char tmpChar[20];
uint16_t rxSize;
bool waitForSave = false;


characterType tmpCharType;
character tmpCharacter;

char myStr[450];
char *end;

void gameSync() {
    if (waitForSave)
        return;

    myStr[0] = GAME_SYNC_SIGNAL_NUMBER;
    end = myStr + 1;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            tmpCharType = *lcdArr(j, i);
            end = stpcpy(end, characterTypeHash(tmpCharType));
        }
    }

    sprintf(&myStr[161], "%03lu", score);

    sprintf(&myStr[164], "%lu", difficulty);

    myStr[165] = (programState == LoseState) ? '1' : '0';

    myStr[GAME_SYNC_MESSAGE_LENGTH] = '\0';
    uartStringTransmit(myStr);
}

bool loadDone = false;

void gameLoad(const char *loadString) {
    char myTmpChar[10] = {0};


    int k = 9;

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            *getCharacter(j, i) = (character) {
                    hashToCharacterType(loadString + k),
                    j, i,
                    (*(loadString + k + 2) == '0') ? false : true};
            k += 3;
        }
    }

    k = 250;

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            *lcdArr(j, i) = hashToCharacterType(loadString + k);
            k += 2;
        }
    }

    myTmpChar[0] = loadString[411];
    difficulty = strtol(myTmpChar, NULL, 10);

    strlcpy(myTmpChar, &loadString[412], 4);

    score = strtol(myTmpChar, NULL, 10);

    doodlerMoveMode = (loadString[415] == '1') ? Ascending : Descending;

    strlcpy(myTmpChar, &loadString[416], 3);

    shiftUpCount = strtol(myTmpChar, NULL, 10);

    myTmpChar[0] = loadString[418];
    myTmpChar[1] = '\0';
    doodler.upper.x = strtol(myTmpChar, NULL, 10);

    myTmpChar[0] = loadString[421];
    myTmpChar[1] = '\0';
    doodler.lower.x = strtol(myTmpChar, NULL, 10);

    strlcpy(myTmpChar, &loadString[419], 3);

    doodler.upper.y = strtol(myTmpChar, NULL, 10);

    strlcpy(myTmpChar, &loadString[422], 3);

    doodler.lower.y = strtol(myTmpChar, NULL, 10);

    loadDone = true;
    //osMutexRelease(gameLoadMutexHandle);
}
//  load-approve-----------
/*
loading-1030200200200020200030200200200200200200200200030200200200050200040200200200200200200200200050200200030200020060200200200200200200200200200030020200200200040200020200050200200200200200200200200040200200020030200200200041200200200200200200a51#2020202002200320202020202020200320202005200420202020202020200520200320020620002020200120202003022020200420022005202020202020202004202002042020202020202020202020#4015009
 */
int gameSave() {
    waitForSave = true;

    for (int i = 0; i < 450; ++i) { // just to ensure it's empty
        myStr[i] = '\0';
    }

    myStr[0] = GAME_SAVE_SIGNAL_NUMBER;
    end = myStr + 1;

    //  characters array 240 characters
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            tmpCharacter = *getCharacter(j, i);
            end = stpcpy(end, characterHash(tmpCharacter));
        }
    }

    myStr[241] = '#';

    end = myStr + 242;
    // lcd array length 160
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            tmpCharType = *lcdArr(j, i);
            end = stpcpy(end, characterTypeHash(tmpCharType));
        }
    }

    myStr[402] = '#';

    sprintf(&myStr[403], "%lu", difficulty);
    sprintf(&myStr[404], "%03lu", score);

    myStr[407] = (doodlerMoveMode == Ascending) ? '1' : '0';

    sprintf(&myStr[408], "%02lu", shiftUpCount);


    sprintf(&myStr[410], "%d", doodler.upper.x);
    sprintf(&myStr[411], "%02d", doodler.upper.y);
    sprintf(&myStr[413], "%d", doodler.lower.x);
    sprintf(&myStr[414], "%02d", doodler.lower.y);

    myStr[416] = '\0';

    uartStringTransmit(myStr);

    osDelay(10); // just to be safe

    waitForSave = false;
    return 0;
}

void uartRxHandle() {

}

void uartRxProcess() {
    rxSize = 24;
    switch (signalHash(uartRxTmpStr)) {
        case ControlLeft:
            doodlerMove(Left);
            break;
        case ControlRight:
            doodlerMove(Right);
            break;
        case ControlFire:
            doodlerGunFire();
            break;
        case ClockSync:
            strlcpy(tmpChar, uartRxTmpStr + 6, 4);
            dateTime.dateVar.Year = strtol(tmpChar, NULL, 10);
            strlcpy(tmpChar, uartRxTmpStr + 9, 4);
            dateTime.dateVar.Month = strtol(tmpChar, NULL, 10);
            strlcpy(tmpChar, uartRxTmpStr + 12, 4);
            dateTime.dateVar.Date = strtol(tmpChar, NULL, 10);

            strlcpy(tmpChar, uartRxTmpStr + 15, 4);
            dateTime.timeVar.Hours = strtol(tmpChar, NULL, 10);
            strlcpy(tmpChar, uartRxTmpStr + 18, 4);
            dateTime.timeVar.Minutes = strtol(tmpChar, NULL, 10);
            strlcpy(tmpChar, uartRxTmpStr + 21, 4);
            dateTime.timeVar.Seconds = strtol(tmpChar, NULL, 10);

            HAL_RTC_SetTime(&hrtc, &dateTime.timeVar, RTC_FORMAT_BIN);
            HAL_RTC_SetDate(&hrtc, &dateTime.dateVar, RTC_FORMAT_BIN);
            updateDateTime();

            break;
        case LoadApprove:
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);
            rxSize = 424; // loading-...
            break;
        case Loading:
            gameLoad(uartRxTmpStr);
            break;
    }
    uartReceive(rxSize);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        osSemaphoreRelease(uartDmaSemHandle);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        osSemaphoreRelease(uartDmaRxSemHandle);
    }
}
