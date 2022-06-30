//
// Created by Sina on 6/27/2022.
//

#ifndef DOODLEJUMP_GLOBALS_H
#define DOODLEJUMP_GLOBALS_H

//#include "cmsis_os.h"
//#include "requisite.h"

typedef enum {
    ASCENDING, DESCENDING
} xChangingMode;

extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart2;

extern PCD_HandleTypeDef hpcd_USB_FS;



extern uint32_t keypadNum;
extern uint32_t score;
extern uint32_t difficulty;


extern char str[100];


extern uint_fast32_t volume;

#endif //DOODLEJUMP_GLOBALS_H
