//
// Created by Sina on 6/30/2022.
//

#ifndef DOODLEJUMP_CHARACTERS_H
#define DOODLEJUMP_CHARACTERS_H

#include "requisite.h"

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
    DoodlerUpDizzy = (uint_fast8_t) 7,
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
    bool dizzy;
} __attribute__((aligned(64))) doodlerType;


extern byte doodlerUpByte[], doodlerUpDizzyByte[], doodlerDownByte[], normalStepByte[], brokenStepByte[], springStepByte[], monsterByte[], holeByte[];
extern character charactersArr2[20][4];
extern doodlerType doodler;
extern character bullets[BULLETS_BUFFER_SIZE];


void shiftDownCharacters();

void generateCharacters();

void gameCharactersInit();

bool collisionWithDoodler(uint32_t x, uint32_t y);

char *characterTypeHash(characterType characterArg);

char *characterHash(character characterArg);

characterType hashToCharacterType(const char *string);


__STATIC_INLINE character *getCharacter(uint_fast8_t x, uint_fast8_t y) {
    return &charactersArr2[y][x];
}

#endif //DOODLEJUMP_CHARACTERS_H
