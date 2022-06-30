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
            probabilities[0] = 1;
            probabilities[1] = 19;
            break;
        case 400 ... 799:
            difficulty = 1;
            probabilities[0] = 2;
            probabilities[1] = 18;
            break;
        case 800 ... 1199:
            difficulty = 2;
            probabilities[0] = 3;
            probabilities[1] = 17;
            break;
        case 1200 ... 1599:
            difficulty = 3;
            probabilities[0] = 4;
            probabilities[1] = 16;
            break;
        case 1600 ... 1999:
            difficulty = 4;
            probabilities[0] = 5;
            probabilities[1] = 15;
            break;
        case 2000 ... 2399:
            difficulty = 5;
            probabilities[0] = 6;
            probabilities[1] = 14;
            break;
        case 2400 ... 2799:
            difficulty = 6;
            probabilities[0] = 7;
            probabilities[1] = 13;
            break;
        case 2800 ... 3199:
            difficulty = 7;
            probabilities[0] = 8;
            probabilities[1] = 12;
            break;
        case 3200 ... 3599:
            difficulty = 8;
            probabilities[0] = 9;
            probabilities[1] = 11;
            break;
        default:
            difficulty = 9;
            probabilities[0] = 10;
            probabilities[1] = 10;
            break;
    }

}