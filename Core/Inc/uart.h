//
// Created by Sina on 7/4/2022.
//

#ifndef DOODLEJUMP_UART_H
#define DOODLEJUMP_UART_H

extern bool loadDone;

void uartFormatTransmit(const char *format, ...);

void uartStringTransmit(const char *string);

void uartReceive(uint16_t size);

void uartRxProcess();

void uartRxHandle();

void gameSync();

int gameSave();

void accelerometerRead();

#endif //DOODLEJUMP_UART_H
