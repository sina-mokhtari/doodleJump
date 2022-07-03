//
// Created by Sina on 7/4/2022.
//

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "requisite.h"
#include "globals.h"

char uartTmpStr[200];

void uartTransmit(const char *format, ...) {
    int len;
    va_list arguments;

    va_start(arguments, format);
    len = vsnprintf(uartTmpStr, strlen(format) + 1, format, arguments);
    va_end(arguments);

    osSemaphoreAcquire(uartDmaSemHandle, osWaitForever);

    HAL_UART_Transmit_DMA(&huart2, (uint8_t *) uartTmpStr, len);
}