//
// Created by Sina on 6/30/2022.
//
#include <math.h>
//#include "requisite.h"
#include "globals.h"
#include "game.h"
#include "adc.h"


uint_fast32_t volumeRawValue;


void adcInit() {
    HAL_ADC_Start_IT(&hadc1);
}

void volumeHandle(ADC_HandleTypeDef *adc) {
    volumeRawValue = HAL_ADC_GetValue(adc);

    switch (volumeRawValue) {
        case 0 ... 399:
            difficulty = 0;
            generateProbability = 40;
            twoStepProbability = 40;
            springStepProbability = 20;
            brokenStepProbability = 10;
            monsterProbability = 4;
            blackHoleProbability = 2;
            break;
        case 400 ... 799:
            difficulty = 1;
            generateProbability = 37;
            twoStepProbability = 37;
            springStepProbability = 19;
            brokenStepProbability = 12;
            monsterProbability = 8;
            blackHoleProbability = 4;
            break;
        case 800 ... 1199:
            difficulty = 2;
            generateProbability = 34;
            twoStepProbability = 34;
            springStepProbability = 18;
            brokenStepProbability = 14;
            monsterProbability = 12;
            blackHoleProbability = 6;
            break;
        case 1200 ... 1599:
            difficulty = 3;
            generateProbability = 31;
            twoStepProbability = 31;
            springStepProbability = 17;
            brokenStepProbability = 16;
            monsterProbability = 16;
            blackHoleProbability = 8;
            break;
        case 1600 ... 1999:
            difficulty = 4;
            generateProbability = 28;
            twoStepProbability = 28;
            springStepProbability = 16;
            brokenStepProbability = 18;
            monsterProbability = 20;
            blackHoleProbability = 10;
            break;
        case 2000 ... 2399:
            difficulty = 5;
            generateProbability = 25;
            twoStepProbability = 25;
            springStepProbability = 15;
            brokenStepProbability = 20;
            monsterProbability = 24;
            blackHoleProbability = 12;
            break;
        case 2400 ... 2799:
            difficulty = 6;
            generateProbability = 22;
            twoStepProbability = 22;
            springStepProbability = 14;
            brokenStepProbability = 22;
            monsterProbability = 28;
            blackHoleProbability = 14;
            break;
        case 2800 ... 3199:
            difficulty = 7;
            generateProbability = 19;
            twoStepProbability = 19;
            springStepProbability = 13;
            brokenStepProbability = 24;
            monsterProbability = 32;
            blackHoleProbability = 16;
            break;
        case 3200 ... 3599:
            difficulty = 8;
            generateProbability = 16;
            twoStepProbability = 16;
            springStepProbability = 12;
            brokenStepProbability = 26;
            monsterProbability = 36;
            blackHoleProbability = 18;

            break;
        default:
            difficulty = 9;
            generateProbability = 13;
            twoStepProbability = 13;
            springStepProbability = 11;
            brokenStepProbability = 28;
            monsterProbability = 40;
            blackHoleProbability = 20;
            break;
    }

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) { // volume
        osSemaphoreRelease(volumeSemHandle);
    }
}