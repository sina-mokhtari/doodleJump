//
// Created by Sina on 7/4/2022.
//

#ifndef DOODLEJUMP_UART_H
#define DOODLEJUMP_UART_H

void uartFormatTransmit(const char *format, ...);

void uartStringTransmit(const char *string);

void uartReceive();

void uartReceiveHandle();

#endif //DOODLEJUMP_UART_H
