//
// Created by Sina on 6/30/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "requisite.h"
#include "characters.h"
#include "lcd.h"

#define SUM_OF_PROBABILITIES 100

uint32_t generateProbability;
uint32_t twoStepProbability;
uint32_t springStepProbability;
uint32_t brokenStepProbability;
uint32_t monsterProbability;
uint32_t blackHoleProbability;


byte doodlerUpByte[] = {
        0x07,
        0x08,
        0x0A,
        0x09,
        0x0A,
        0x08,
        0x07,
        0x00
};

byte doodlerDownByte[] = {
        0x0E,
        0x15,
        0x15,
        0x15,
        0x15,
        0x15,
        0x0E,
        0x00
};
byte normalStepByte[] = {
        0x10,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x10,
        0x00
};
byte brokenStepByte[] = {
        0x10,
        0x18,
        0x18,
        0x10,
        0x08,
        0x18,
        0x10,
        0x00
};
byte springStepByte[] = {
        0x02,
        0x03,
        0x13,
        0x1F,
        0x13,
        0x03,
        0x02,
        0x00
};
byte monsterByte[] = {
        0x17,
        0x18,
        0x08,
        0x0A,
        0x08,
        0x18,
        0x17,
        0x00
};
byte holeByte[] = {
        0x04,
        0x0E,
        0x1F,
        0x1F,
        0x1F,
        0x0E,
        0x04,
        0x00
};

character charactersArr[CHARACTERS_NUMBER];
doodlerType doodler;


void charactersInit() {
    int j;
    for (int i = 0, y = -1; i < CHARACTERS_NUMBER; i++) {
        j = i % VERTICAL_LCD_COLUMNS;
        *getCharacter(i) = (character) {Air, j, (j != 0) ? y : ++y,false};
    }
    doodler.upper = (character) {DoodlerUp, 2, 9,false};
    doodler.lower = (character) {DoodlerDown, 2, 10,false};

    *getCharacter(18) = (character) {SpringStep, 2, 4, false};
    *getCharacter(38) = (character) {NormalStep, 2, 9, false};
    *getCharacter(54) = (character) {NormalStep, 2, 13, false};
}

character *findCharacter(uint_fast8_t x, uint_fast8_t y) {

#pragma unroll
    for (int i = 0; i < CHARACTERS_NUMBER; i++) {
        if (getCharacter(i)->y == y &&
            getCharacter(i)->x == x) {
            return getCharacter(i);
        }
    }
    return NULL;
}

characterType randomAdditional() {
#pragma unroll
    for (int i = 0; i < 4; i++) {
        switch (random() % 4) {
            case 0:
                if (random() % SUM_OF_PROBABILITIES < brokenStepProbability)
                    return BrokenStep;
                break;
            case 1:
                if (random() % SUM_OF_PROBABILITIES < springStepProbability)
                    return SpringStep;
                break;
            case 2:
                if (random() % SUM_OF_PROBABILITIES < monsterProbability)
                    return Monster;
                break;
            default :
                if (random() % SUM_OF_PROBABILITIES < blackHoleProbability)
                    return BlackHole;
                break;
        }
    }
    return Air;
}


character randArr[10] = {[0 ... 9] = NormalStep};
bool foundRandom;

void replaceNewCharacters(uint32_t n) {

    for (int i = 0; i <= n; i++) {
        randArr[i].x = random() % VERTICAL_LCD_COLUMNS;
        for (int j = 0; j < i; j++) {
            if (randArr[i].x == randArr[j].x) {
                i--;
                break;
            }
        }
        randArr[i].type = NormalStep;
    }

    randArr[n].type = randomAdditional();

    for (int i = 0, j = 0; j < VERTICAL_LCD_COLUMNS && i < CHARACTERS_NUMBER; i++) {
        if (getCharacter(i)->y == VERTICAL_LCD_ROWS) {
            foundRandom = false;

#pragma unroll 3
            for (int l = 0; l <= n; l++) {
                if (j == randArr[l].x) {
                    *getCharacter(i) = (character) {randArr[l].type, j, 0, false};
                    foundRandom = true;
                    break;
                }
            }
            if (!foundRandom)
                *getCharacter(i) = (character) {Air, j, 0,false};
            j++;
        }
    }
}


uint_fast8_t distanceToLastStep;

void generateCharacters() {

    if ((random() % SUM_OF_PROBABILITIES) < generateProbability) { // generate
        if ((random() % SUM_OF_PROBABILITIES) < twoStepProbability) // generate 2 steps
            replaceNewCharacters(2);
        else    // generate 1 step
            replaceNewCharacters(1);
        distanceToLastStep = 0;
    } else if (distanceToLastStep < 6) { // not generate
        replaceNewCharacters(0);
        distanceToLastStep++;
    } else {
        replaceNewCharacters(1);
        distanceToLastStep = 0;
    }
}

void shiftDownCharacters(uint32_t shiftStep) {
    for (int i = 0; i < CHARACTERS_NUMBER; i++) {
        if (getCharacter(i)->type != Bullet)
            getCharacter(i)->y += shiftStep;

    }
    generateCharacters();
}

