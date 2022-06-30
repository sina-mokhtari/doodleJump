//
// Created by Sina on 6/30/2022.
//
#include <math.h>
//#include "requisite.h"
#include "adc.h"


uint_fast32_t volumeRawValue;


void adcInit() {
    HAL_ADC_Start_IT(&hadc1);
}

void handleVolume(ADC_HandleTypeDef *hadc) {
    volumeRawValue = HAL_ADC_GetValue(hadc);

    switch (volumeRawValue) {
        case 0 ... 399:
            difficulty = 0;
            break;
        case 400 ... 799:
            difficulty = 1;
            break;
        case 800 ... 1199:
            difficulty = 2;
            break;
        case 1200 ... 1599:
            difficulty = 3;
            break;
        case 1600 ... 1999:
            difficulty = 4;
            break;
        case 2000 ... 2399:
            difficulty = 5;
            break;
        case 2400 ... 2799:
            difficulty = 6;
            break;
        case 2800 ... 3199:
            difficulty = 7;
            break;
        case 3200 ... 3599:
            difficulty = 8;
            break;
        default:
            difficulty = 9;
            break;
    }

}