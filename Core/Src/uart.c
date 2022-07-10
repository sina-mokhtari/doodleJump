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

#define GAME_SYNC_SIGNAL_NUMBER 0
#define GAME_SAVE_SIGNAL_NUMBER 1

#define GAME_SYNC_MESSAGE_LENGTH 161
#define GAME_SAVE_MESSAGE_LENGTH 1000

typedef enum {
    ControlLeft, ControlRight, ControlFire, ClockSync, LoadApprove
} uartSignalType;


char uartTmpStr[100];
char uartRxTmpStr[50];

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

    } else if (strncmp(uartRxTmpStr, "clock-", 6) == 0)
        return ClockSync;

    else if (strncmp(uartRxTmpStr, "load-approve", 12) == 0)
        return LoadApprove;

    return (uartSignalType) NULL;
}

char tmpChar[20];
uint16_t rxSize;

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
            break;
    }
    uartReceive(rxSize);
}

characterType tmpCharType;
character tmpCharacter;

char myStr[450];
char *end;

void gameSync() {
    myStr[0] = GAME_SYNC_SIGNAL_NUMBER;
    end = myStr + 1;
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            tmpCharType = *lcdArr(j, i);
            end = stpcpy(end, characterTypeHash(tmpCharType));
        }
    }
    //myStr[175] = '#';
    myStr[GAME_SYNC_MESSAGE_LENGTH] = '\0';
    uartStringTransmit(myStr);
}

int gameSave() {
    //char charTestT[38] = "hellon08yn8b76tb7v6b5r 566b 65iud5irb\0";

    /*for (int i = 0; i < 15; ++i)
        myStr[i] = '$';  // space

    for (int i = 175; i < 180; ++i)
        myStr[i] = '#';  // space*/

    /* for (int i = 1; i < 50; ++i) {
         myStr[i] = '%';
     }
 */
    //myStr[59] = '\n';
//    if(myStr[0] == '*') {
//        myStr[0] = '@';
//        myStr[49] = '!';
//    } else{
//        myStr[0] = '*';
//        myStr[49] = '&';
//    }
//    myStr[50] = 0;
//-----------------------------------------
    //myStr[60] = 0;
//    myStr[0] =
//    end = myStr;
//    for (int i = 0; i < 20; ++i) {
//        for (int j = 0; j < 4; ++j) {
//            tmpCharType = *lcdArr(j, i);
//            end = stpcpy(end, characterTypeHash(tmpCharType));
//        }
//    }
//    //myStr[175] = '#';
//    myStr[160] = 0;
//    uartStringTransmit(myStr);
//    osDelay(1000);
//--------------------------------------------

    // uartStringTransmit(myStr);
/*    osDelay(100);
     end = myStr + 15;
     for (int i = 0; i < 5; ++i) {
         for (int j = 0; j < 4; ++j) {
             tmpCharType = *lcdArr(j, i);
             end = stpcpy(end, characterTypeHash(tmpCharType));
         }
     }
     myStr[55] = '0';
 //    myStr[175] = '*'; // space
 //    myStr[180] = 0; // '\0'
     uartStringTransmit(myStr);  // sending lcdArr 0th
     //BSP_LED_On()
     osDelay(100);

     end = myStr + 15;
     for (int i = 5; i < 10; ++i) {
         for (int j = 0; j < 4; ++j) {
             tmpCharType = *lcdArr(j, i);
             end = stpcpy(end, characterTypeHash(tmpCharType));
         }
     }
     myStr[55] = '1';
     uartStringTransmit(myStr);  // sending lcdArr 1th
     osDelay(100);

     end = myStr + 15;
     for (int i = 10; i < 15; ++i) {
         for (int j = 0; j < 4; ++j) {
             tmpCharType = *lcdArr(j, i);
             end = stpcpy(end, characterTypeHash(tmpCharType));
         }
     }

     myStr[55] = '2';
     uartStringTransmit(myStr);  // sending lcdArr 2th
     osDelay(100);

     end = myStr + 15;
     for (int i = 15; i < 20; ++i) {
         for (int j = 0; j < 4; ++j) {
             tmpCharType = *lcdArr(j, i);
             end = stpcpy(end, characterTypeHash(tmpCharType));
         }
     }

     myStr[55] = '3';
     uartStringTransmit(myStr);  // sending lcdArr 3th
     // BSP_GYRO_GetXYZ(pDataXyz);
     //sprintf(charTestT, "%f\n", pDataXyz[0]);

     //uartStringTransmit(charTestT);

*/

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

    sprintf(&myStr[403],"%lu",difficulty);
    sprintf(&myStr[404], "%03lu", score);

    myStr[407] = (doodlerMoveMode == Ascending) ? '0' : '1';

    sprintf(&myStr[408],"%02lu",shiftUpCount);


    uartStringTransmit(myStr);  // sending characterArr

    // accelerometerRead();
    // osDelay(100);
    return 0;
}

void uartRxHandle() {

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
