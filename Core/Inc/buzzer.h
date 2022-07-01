//
// Created by Sina on 7/1/2022.
//

#ifndef DOODLEJUMP_BUZZER_H
#define DOODLEJUMP_BUZZER_H

#include "requisite.h"

typedef enum {
    JumpLittle, JumpBig, Intro, FireBall, OneUp
} melodyName;

void buzzerChangeTone(uint16_t freq, uint16_t volume);

void buzzerInit();

void buzzerMelodyIntro();

void buzzerMelodyFireBall();

void buzzerMelody1Up();

void buzzerMelodyJumpLittle();

void buzzerMelodyJumpBig();

void buzzerMelodyPlay(melodyName melodyToPlay);

#endif //DOODLEJUMP_BUZZER_H
