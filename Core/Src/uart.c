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
#include "timeManagement.h"

char uartTmpStr[250];
char uartRxTmpStr[200];

void uartFormatTransmit(const char *format, ...) {
    int len;
    va_list arguments;

    va_start(arguments, format);
    len = vsnprintf(uartTmpStr, strlen(format) + 1, format, arguments);
    va_end(arguments);

    osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);

    HAL_UART_Transmit_DMA(&huart2, (uint8_t *) uartTmpStr, len);
}

void uartStringTransmit(const char *string) {

    osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);

    HAL_UART_Transmit_DMA(&huart2, (uint8_t *) string, strlen(string));
}

void uartReceive() {
    HAL_UART_Receive_DMA(&huart2, (uint8_t *) uartRxTmpStr, 25);
}


char tmpChar[20];

void uartReceiveHandle() {
    if (strncmp(uartRxTmpStr, "clockk-", 7) == 0) {  //  "clockk-22/07/07-20:08:13"
        dateTime.dateVar.Year = atoi(strncpy(tmpChar, uartRxTmpStr + 7, 2));
        dateTime.dateVar.Month = atoi(strncpy(tmpChar, uartRxTmpStr + 10, 2));
        dateTime.dateVar.Date = atoi(strncpy(tmpChar, uartRxTmpStr + 13, 2));

        dateTime.timeVar.Hours = atoi(strncpy(tmpChar, uartRxTmpStr + 16, 2));
        dateTime.timeVar.Minutes = atoi(strncpy(tmpChar, uartRxTmpStr + 19, 2));
        dateTime.timeVar.Seconds = atoi(strncpy(tmpChar, uartRxTmpStr + 22, 2));

        HAL_RTC_SetTime(&hrtc, &dateTime.timeVar, RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc, &dateTime.dateVar, RTC_FORMAT_BIN);

        updateDateTime();

    } else if (strncmp(uartRxTmpStr, "control-", 8) == 0) {
        if (strncmp(uartRxTmpStr + 8, "left", 4) == 0) {  // control-left------------
            doodlerMove(Left);
        } else if (strncmp(uartRxTmpStr + 8, "right", 5) == 0) {  //  control-right-----------
            doodlerMove(Right);
        } else if (strncmp(uartRxTmpStr + 8, "fire", 4) == 0) {  // control-fire------------
            doodlerGunFire();
        }
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        osSemaphoreRelease(uartDmaSemHandle);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        osSemaphoreRelease(uartDmaRxSemHandle);
    }
}