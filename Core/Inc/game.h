//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_GAME_H
#define DOODLEJUMP_GAME_H

#include "requisite.h"

#define HIGH_JUMP_HEIGHT 20
#define NORMAL_JUMP_HEIGHT 7


typedef enum {
    IntroState,
    MenuState,
    PlayingState,
    LoseState,
    AboutState,
    SettingsState
} programStateType;

typedef enum {
    Ascending, Descending
} doodlerMoveModeType;

typedef enum {
    Up, Down, Left, Right
} doodlerMoveDirectionType;

extern programStateType programState;

extern uint32_t score;

extern uint32_t difficulty;

extern doodlerMoveModeType doodlerMoveMode;

extern uint32_t shiftUpCount;

int programRun();

void gameStart();

void doodlerMove(doodlerMoveDirectionType direction);

void addScore();

void resetScore();

void doodlerGunFire();

void playingStateKeypadHandle();


#endif //DOODLEJUMP_GAME_H
