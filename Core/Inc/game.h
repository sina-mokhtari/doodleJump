//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_GAME_H
#define DOODLEJUMP_GAME_H

#include "requisite.h"

#define HIGH_JUMP_HEIGHT 20
#define NORMAL_JUMP_HEIGHT 7

typedef enum {
    Ascending, Descending
} doodlerMoveModeType;

typedef enum {
    Up, Down, Left, Right
} doodlerMoveDirectionType;

int gameHandle();

void gameStart();

void doodlerMove(doodlerMoveDirectionType direction);

void addScore();

uint32_t getScore();

#endif //DOODLEJUMP_GAME_H
