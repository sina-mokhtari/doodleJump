//
// Created by Sina on 7/1/2022.
//


#include "buzzer.h"
#include "cmsis_os.h"

#define DEFAULT_VOLUME 1


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

TIM_HandleTypeDef *buzzerPwmTimer = &htim8;
uint32_t buzzerPwmChannel = TIM_CHANNEL_1;

void buzzerChangeTone(uint16_t freq, uint16_t volume) {
    if (freq == 0 || freq > 20000) {
        __HAL_TIM_SET_COMPARE(buzzerPwmTimer, buzzerPwmChannel, 0);
    } else {
        const uint32_t internalClockFreq = HAL_RCC_GetSysClockFreq();
        const uint32_t prescaler = 1 + internalClockFreq / freq / 60000;
        const uint32_t timerClock = internalClockFreq / prescaler;
        const uint32_t periodCycles = timerClock / freq;
        const uint32_t pulseWidth = volume * periodCycles / 1000 / 2;

        buzzerPwmTimer->Instance->PSC = prescaler - 1;
        buzzerPwmTimer->Instance->ARR = periodCycles - 1;
        buzzerPwmTimer->Instance->EGR = TIM_EGR_UG;

        __HAL_TIM_SET_COMPARE(buzzerPwmTimer, buzzerPwmChannel, pulseWidth);
    }
}

void buzzerInit() {
    HAL_TIM_PWM_Start(buzzerPwmTimer, buzzerPwmChannel);
}


const int introMelody[] = {
        NOTE_E7, NOTE_E7, 0, NOTE_E7,
        0, NOTE_C7, NOTE_E7, 0,
        NOTE_G7, 0, 0, 0,
        NOTE_G6, 0, 0, 0,

        NOTE_C7, 0, 0, NOTE_G6,
        0, 0, NOTE_E6, 0,
        0, NOTE_A6, 0, NOTE_B6,
        0, NOTE_AS6, NOTE_A6, 0,

        NOTE_G6, NOTE_E7, NOTE_G7,
        NOTE_A7, 0, NOTE_F7, NOTE_G7,
        0, NOTE_E7, 0, NOTE_C7,
        NOTE_D7, NOTE_B6, 0, 0,

        NOTE_C7, 0, 0, NOTE_G6,
        0, 0, NOTE_E6, 0,
        0, NOTE_A6, 0, NOTE_B6,
        0, NOTE_AS6, NOTE_A6, 0,

        NOTE_G6, NOTE_E7, NOTE_G7,
        NOTE_A7, 0, NOTE_F7, NOTE_G7,
        0, NOTE_E7, 0, NOTE_C7,
        NOTE_D7, NOTE_B6, 0, 0
};

const int introMelodyTempo[] = {
        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 132,

        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 154,

        143, 143, 143,//
        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 275,

        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 154,

        143, 143, 143,//
        132, 132, 132, 132,
        132, 132, 132, 132,
        132, 132, 132, 275,
};

void buzzerMelodyJumpLittle() {
    buzzerChangeTone(NOTE_B5, DEFAULT_VOLUME);
    osDelay(100);
    buzzerChangeTone(NOTE_E6, DEFAULT_VOLUME);
    osDelay(400);
    buzzerChangeTone(REST, 0);
}

void buzzerMelodyJumpBig() {
    buzzerChangeTone(NOTE_B5, DEFAULT_VOLUME);
    osDelay(100);
    buzzerChangeTone(NOTE_G6, DEFAULT_VOLUME);
    osDelay(400);
    buzzerChangeTone(REST, 0);
}

void buzzerMelody1Up() {
    buzzerChangeTone(NOTE_E6, DEFAULT_VOLUME);
    osDelay(130);
    buzzerChangeTone(NOTE_G6, DEFAULT_VOLUME);
    osDelay(130);
    buzzerChangeTone(NOTE_E7, DEFAULT_VOLUME);
    osDelay(130);
    buzzerChangeTone(NOTE_C7, DEFAULT_VOLUME);
    osDelay(130);
    buzzerChangeTone(NOTE_D7, DEFAULT_VOLUME);
    osDelay(130);
    buzzerChangeTone(NOTE_G7, DEFAULT_VOLUME);
    osDelay(125);
    buzzerChangeTone(REST, 0);
}

void buzzerMelodyFireBall() {
    buzzerChangeTone(NOTE_G4, DEFAULT_VOLUME);
    osDelay(35);
    buzzerChangeTone(NOTE_G5, DEFAULT_VOLUME);
    osDelay(35);
    buzzerChangeTone(NOTE_G6, DEFAULT_VOLUME);
    osDelay(35);
    buzzerChangeTone(REST, 0);
}

void buzzerMelodyIntro() {
    for (int i = 0; i < 78; i++) {
        buzzerChangeTone(introMelody[i], DEFAULT_VOLUME);
        osDelay(introMelodyTempo[i]);
    }
    buzzerChangeTone(REST, 0);
}

void buzzerMelodyPlay(melodyName melodyToPlay) {

    switch (melodyToPlay) {
        case Intro:
            buzzerMelodyIntro();
            break;
        case JumpBig:
            buzzerMelodyJumpBig();
            break;
        case JumpLittle:
            buzzerMelodyJumpLittle();
            break;
        case FireBall:
            buzzerMelodyFireBall();
            break;
        case OneUp:
            buzzerMelody1Up();
            break;
        default:
            break;
    }
}