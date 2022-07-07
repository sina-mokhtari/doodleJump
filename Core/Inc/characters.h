//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_CHARACTERS_H
#define DOODLEJUMP_CHARACTERS_H

#include "requisite.h"

#define CHARACTERS_NUMBER 80
#define BULLETS_BUFFER_SIZE 10

typedef enum {
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
    bool characterFlag;
} __attribute__((aligned(16))) character;

typedef struct {
    character upper;
    character lower;
} __attribute__((aligned(32))) doodlerType;


extern byte doodlerUpByte[], doodlerDownByte[], normalStepByte[], brokenStepByte[], springStepByte[], monsterByte[], holeByte[];
extern character charactersArr[80];
extern character charactersArr2[20][4];
extern doodlerType doodler;
extern character bullets[BULLETS_BUFFER_SIZE];


void shiftDownCharacters(uint32_t shiftStep);

void generateCharacters();

void charactersInit();

character *findCharacter(uint_fast8_t x, uint_fast8_t y);

bool collisionWithDoodler(uint32_t x, uint32_t y);


__STATIC_INLINE character *getCharacter(uint_fast8_t i) {
    return &charactersArr[i];
}

__STATIC_INLINE character *getCharacter2(uint_fast8_t x, uint_fast8_t y) {
    return &charactersArr2[y][x];
}


#endif //DOODLEJUMP_CHARACTERS_H
