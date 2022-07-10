//
// Created by Sina on 7/9/2022.
//

#include "requisite.h"
#include "../../BSP/STM32F3-Discovery/stm32f3_discovery_accelerometer.h"
#include "../../BSP/STM32F3-Discovery/stm32f3_discovery_gyroscope.h"
#include "game.h"

#define ABS(x)  (((x) < 0) ? -(x) : (x))

#define THRESHOLD_HIGH  5000
#define THRESHOLD_LOW  (-5000)

bool accelerometerControlEnabled = false;


void accelerometerInit() {
    BSP_ACCELERO_Init();
}

void gyroscopeInit() {
    BSP_GYRO_Init();
}

uint32_t lastAccelerometerRead = 0;

void accelerometerRead() {

    int16_t pDataXyz[3] = {0};
    int16_t xVal, yVal;

    /* Read Acceleration*/
    __disable_irq();
    BSP_ACCELERO_GetXYZ(pDataXyz);
    __enable_irq();

    /* Update auto reload and capture compare registers value*/
    xVal = pDataXyz[0];
    yVal = pDataXyz[1];

    if (ABS(xVal) > ABS(yVal)) {
        if (xVal > THRESHOLD_HIGH) { // backward
            /* LED7 On */
            BSP_LED_On(LED3);
            osDelay(10);
        } else if (xVal < THRESHOLD_LOW) { // forward
            /* LED6 On */
            BSP_LED_On(LED10);
            osDelay(10);
        } else {
            osDelay(10);
        }
    } else {
        if (yVal < THRESHOLD_LOW) { // left
            /* LED3 On */
            BSP_LED_On(LED7);
            osDelay(10);
            if (accelerometerControlEnabled) {
                if (lastAccelerometerRead + 500 < osKernelGetTickCount()) {
                    doodlerMove(Left);
                    lastAccelerometerRead = osKernelGetTickCount();
                }
            }
        } else if (yVal > THRESHOLD_HIGH) { // right
            /* LED10 On */
            BSP_LED_On(LED6);
            osDelay(10);
            if (accelerometerControlEnabled) {
                if (lastAccelerometerRead + 500 < osKernelGetTickCount()) {
                    doodlerMove(Right);
                    lastAccelerometerRead = osKernelGetTickCount();
                }
            }
        } else {
            osDelay(10);
        }
    }

    BSP_LED_Off(LED3);
    BSP_LED_Off(LED6);
    BSP_LED_Off(LED7);
// BSP_LED_Off(LED4);
    BSP_LED_Off(LED10);
//BSP_LED_Off(LED8);
//BSP_LED_Off(LED9);
//BSP_LED_Off(LED5);
}

void gyroRead() {

    float pDataXyz[3] = {0};
    float xVal, yVal;

/* Read Gyro Angular data */
    BSP_GYRO_GetXYZ(pDataXyz);

/* Update auto reload and capture compare registers value*/
    xVal = ABS((pDataXyz[0]));
    yVal = ABS((pDataXyz[1]));

    if (xVal > yVal) {
        if (pDataXyz[0] > 5000.0f) {
/* LD10 On */
            BSP_LED_On(LED10);
            osDelay(10);
        } else if (pDataXyz[0] < -5000.0f) {
/* LED3 On */
            BSP_LED_On(LED3);
            osDelay(10);
        } else {
            osDelay(10);
        }
    } else {
        if (pDataXyz[1] < -5000.0f) {
/* LD6 on */
            BSP_LED_On(LED6);
            osDelay(10);
        } else if (pDataXyz[1] > 5000.0f) {
/* LD7 On */
            BSP_LED_On(LED7);
            osDelay(10);
        } else {
            osDelay(10);
        }
    }
    BSP_LED_Off(LED3);
    BSP_LED_Off(LED6);
    BSP_LED_Off(LED7);
    BSP_LED_Off(LED4);
    BSP_LED_Off(LED10);
    BSP_LED_Off(LED8);
    BSP_LED_Off(LED9);
    BSP_LED_Off(LED5);
}
