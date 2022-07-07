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
    Menu,
    Playing,
    Losing
} gameStateType;

typedef enum {
    Ascending, Descending
} doodlerMoveModeType;

typedef enum {
    Up, Down, Left, Right
} doodlerMoveDirectionType;

extern gameStateType gameStat;

int gameHandle();

void gameStart();

void doodlerMove(doodlerMoveDirectionType direction);

void addScore();

uint32_t getScore();

void doodlerGunFire();


#endif //DOODLEJUMP_GAME_H
