//
// Created by Sina on 7/7/2022.
//

#include "timeManagement.h"


dateTimeType dateTime;

void updateDateTime() {
    HAL_RTC_GetTime(&hrtc, &dateTime.timeVar, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &dateTime.dateVar, RTC_FORMAT_BIN);
}

dateTimeType getDateTime() {
    return dateTime;
}