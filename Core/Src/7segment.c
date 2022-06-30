//
// Created by Sina on 6/30/2022.
//

#include "7segment.h"
#include "cmsis_os.h"

// order : D1 , D2 , D3 , D4 , A , B , C , D , DP
GPIO_TypeDef *const sevenSegmentPorts[] = {GPIOA, GPIOA, GPIOA, GPIOA, GPIOC, GPIOC, GPIOB, GPIOB, GPIOB};
const uint16_t sevenSegmentPins[] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_0,
                                     GPIO_PIN_1, GPIO_PIN_2};

void decimalToBCD(uint32_t n, uint32_t dp, uint16_t Dx) {
    HAL_GPIO_WritePin(sevenSegmentPorts[0], Dx, 1); // the port just has to be one of D1,D2,D3,D4 ports
    HAL_GPIO_WritePin(sevenSegmentPorts[0], ~Dx, 0); // because they need to be on same port as they are
    HAL_GPIO_WritePin(sevenSegmentPorts[4], sevenSegmentPins[4], n & 1);
    HAL_GPIO_WritePin(sevenSegmentPorts[5], sevenSegmentPins[5], n & 2);
    HAL_GPIO_WritePin(sevenSegmentPorts[6], sevenSegmentPins[6], n & 4);
    HAL_GPIO_WritePin(sevenSegmentPorts[7], sevenSegmentPins[7], n & 8);
    HAL_GPIO_WritePin(sevenSegmentPorts[8], sevenSegmentPins[8], !dp);
}

void displayOneFrame(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    decimalToBCD(a, 1, sevenSegmentPins[0]);
    osDelay(1);
    decimalToBCD(b, 0, sevenSegmentPins[1]);
    osDelay(1);
    decimalToBCD(c, 0, sevenSegmentPins[2]);
    osDelay(1);
    decimalToBCD(d, 0, sevenSegmentPins[3]);
    osDelay(1);
}

uint32_t digit1, digit2, digit3, tmp7segment;

void update7Segment(uint32_t s, uint32_t d) {
    tmp7segment = s;
    digit1 = tmp7segment % 10;
    tmp7segment /= 10;
    digit2 = tmp7segment % 10;
    tmp7segment /= 10;
    digit3 = tmp7segment % 10;

    displayOneFrame(d, digit3, digit2, digit1);
}