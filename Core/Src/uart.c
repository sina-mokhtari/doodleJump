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

char uartTmpStr[200];
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


char *tmpChar;
void uartReceiveHandle() {
    if (strncmp(uartRxTmpStr, "clockk-", 7) == 0) {  //  "clockk-2022/07/07-20:08:13"
        date.Year = atoi(strncpy(tmpChar, uartRxTmpStr + 7, 4));
        date.Month = atoi(strncpy(tmpChar, uartRxTmpStr + 12, 2));
        date.Date = atoi(strncpy(tmpChar, uartRxTmpStr + 15, 2));

        time.Hours = atoi(strncpy(tmpChar, uartRxTmpStr + 18, 2));
        time.Minutes = atoi(strncpy(tmpChar, uartRxTmpStr + 21, 2));
        time.Seconds = atoi(strncpy(tmpChar, uartRxTmpStr + 24, 2));

        HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
        HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);

    } else if (strncmp(uartRxTmpStr, "control", 7) == 0) {
        if (strncmp(uartRxTmpStr + 7, "left", 4) == 0) {
            doodlerMove(Left);
        } else if (strncmp(tmpChar + 7, "right", 5) == 0) {
            doodlerMove(Right);
        } else if (strncmp(tmpChar + 7, "fire", 4) == 0) {
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