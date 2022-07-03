//
// Created by Sina on 6/27/2022.
//

#ifndef DOODLEJUMP_GLOBALS_H
#define DOODLEJUMP_GLOBALS_H

#include "cmsis_os.h"
//#include "requisite.h"

extern ADC_HandleTypeDef hadc1;

extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim8;

extern PCD_HandleTypeDef hpcd_USB_FS;

extern osThreadId_t playMelodyTskHandle;
extern osMessageQueueId_t melodyNameQuHandle;
extern osThreadId_t updateLcdTskHandle;

extern osSemaphoreId_t uartDmaSemHandle;

extern uint32_t keypadNum;
extern uint32_t difficulty;
extern uint32_t generateProbability;
extern uint32_t twoStepProbability;
extern uint32_t springStepProbability;
extern uint32_t brokenStepProbability;
extern uint32_t monsterProbability;
extern uint32_t blackHoleProbability;



#endif //DOODLEJUMP_GLOBALS_H
