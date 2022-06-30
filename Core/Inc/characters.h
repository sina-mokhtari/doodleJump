//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_CHARACTERS_H
#define DOODLEJUMP_CHARACTERS_H

#include "requisite.h"

extern byte doodlerUpByte[], doodlerDownByte[], normalStepByte[], brokenStepByte[], springStepByte[], monsterByte[], holeByte[];

typedef enum {
    AIR = (uint_fast8_t) 0x20,
    DOODLER_UP = (uint_fast8_t) 0,
    DOODLER_DOWN = (uint_fast8_t) 1,
    NORMAL_STEP = (uint_fast8_t) 2,
    BROKEN_STEP = (uint_fast8_t) 3,
    SPRING_STEP = (uint_fast8_t) 4,
    MONSTER = (uint_fast8_t) 5,
    HOLE = (uint_fast8_t) 6,
    BULLET = (uint_fast8_t) 0xa5
} characterType;

typedef struct {
    characterType type;
    uint_fast8_t x;
    uint_fast8_t y;
} Character;

extern Character characters[82];

void shiftDownCharacters(uint32_t shiftStep);

void generateCharacters();

void charactersInit();

#endif //DOODLEJUMP_CHARACTERS_H
