//
// Created by Sina on 6/27/2022.
//

#ifndef DOODLEJUMP_ADC_H
#define DOODLEJUMP_ADC_H

#include "requisite.h"

#define VOLUME_DEFAULT 10
#define VOLUME_FILTERING_SAMPLE_NUM 16
#define ADC_DELAY 20

void handleVolume(ADC_HandleTypeDef *adc);
void adcInit();

#endif //DOODLEJUMP_ADC_H
