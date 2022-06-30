//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_GAME_H
#define DOODLEJUMP_GAME_H


typedef enum {
    ASCENDING, DESCENDING
} doodlerMoveModeType;

int handleGame();
void doodlerMoveLeft();
void doodlerMoveRight();
#endif //DOODLEJUMP_GAME_H
