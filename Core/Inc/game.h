//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_GAME_H
#define DOODLEJUMP_GAME_H

#include "requisite.h"

typedef enum {
    ASCENDING, DESCENDING
} doodlerMoveModeType;

int handleGame();
void gameStart();
void doodlerMoveLeft();
void doodlerMoveRight();
void addScore();
uint32_t getScore();
#endif //DOODLEJUMP_GAME_H
