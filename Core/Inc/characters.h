//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_CHARACTERS_H
#define DOODLEJUMP_CHARACTERS_H

#include "requisite.h"

extern byte doodlerUpByte[], doodlerDownByte[], normalStepByte[], brokenStepByte[], springStepByte[], monsterByte[], holeByte[];

typedef enum {
    DefaultCharacterType,
    Air = (uint_fast8_t) 0x20,
    DoodlerUp = (uint_fast8_t) 0,
    DoodlerDown = (uint_fast8_t) 1,
    NormalStep = (uint_fast8_t) 2,
    BrokenStep = (uint_fast8_t) 3,
    SpringStep = (uint_fast8_t) 4,
    Monster = (uint_fast8_t) 5,
    BlackHole = (uint_fast8_t) 6,
    Bullet = (uint_fast8_t) 0xa5
} characterType;

typedef struct {
    characterType type;
    uint_fast8_t x;
    uint_fast8_t y;
} character;

extern character characters[80];
extern character doodler[2];



void shiftDownCharacters(uint32_t shiftStep);

void generateCharacters();

void charactersInit();

#endif //DOODLEJUMP_CHARACTERS_H
