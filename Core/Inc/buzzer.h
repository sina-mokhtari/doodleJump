//
// Created by Sina on 7/1/2022.
//

#ifndef DOODLEJUMP_BUZZER_H
#define DOODLEJUMP_BUZZER_H

#include "requisite.h"

typedef enum {
    MelodyJumpLittle, MelodyJumpBig, MelodyIntro, MelodyFireBall, OneUp, MelodyFall,MelodyBlackHole
} melodyName;

extern bool buzzerPlayingMelody;

void buzzerChangeTone(uint16_t freq, uint16_t volume);

void buzzerInit();

void melodyQueueSend(melodyName name);

void buzzerMelodyIntro();

void buzzerMelodyFireBall();

void buzzerMelody1Up();

void buzzerMelodyJumpLittle();

void buzzerMelodyJumpBig();

void buzzerMelodyLose();

void buzzerMelodyPlay(melodyName melodyToPlay);

#endif //DOODLEJUMP_BUZZER_H
