//
// Created by Sina on 6/30/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "requisite.h"
#include "characters.h"
#include "lcd.h"
#include "uart.h"
#include "LiquidCrystal.h"

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
character charactersArr2[20][4];
doodlerType doodler;
character bullets[BULLETS_BUFFER_SIZE];

bool collisionWithDoodler(uint32_t x, uint32_t y) {
    return (x == doodler.upper.x && y == doodler.upper.y) ||
           (x == doodler.lower.x && y == doodler.lower.y);
}

void charactersInit() {
    int j;
    for (int i = 0, y = -1; i < CHARACTERS_NUMBER; i++) {
        j = i % VERTICAL_LCD_COLUMNS;
        *getCharacter(i) = (character) {Air, j, (j != 0) ? y : ++y, false};
    }

    doodler.upper = (character) {DoodlerUp, 2, 9, false};
    doodler.lower = (character) {DoodlerDown, 2, 10, false};

    for (int i = 0; i < BULLETS_BUFFER_SIZE; ++i) {
        bullets[i].characterFlag = false;
    }

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

uint32_t myBits = 0;
uint32_t numberCounter = 0;
uint32_t numberTest = 0;

characterType randomRecursive() {
    numberCounter++;
    if (myBits == 0xf || numberCounter > 20) {
        myBits = 0;
        numberTest = numberCounter;
        numberCounter = 0;
        return Air;
    }
    switch (random() % 4) {
        case 0:
            if (((myBits & 1) == 0) && (random() % SUM_OF_PROBABILITIES < brokenStepProbability)) {
                myBits |= 1;
                numberTest = numberCounter;
                numberCounter = 0;
                return BrokenStep;
            } else
                return randomRecursive();
        case 1:
            if (((myBits & 2) == 0) && (random() % SUM_OF_PROBABILITIES < springStepProbability)) {
                myBits |= 2;
                numberTest = numberCounter;
                numberCounter = 0;
                return SpringStep;
            } else
                return randomRecursive();
        case 2:
            if (((myBits & 4) == 0) && (random() % SUM_OF_PROBABILITIES < monsterProbability)) {
                myBits |= 4;
                numberTest = numberCounter;
                numberCounter = 0;
                return Monster;
            } else
                return randomRecursive();
        default :
            if (((myBits & 8) == 0) && (random() % SUM_OF_PROBABILITIES < blackHoleProbability)) {
                myBits |= 8;
                numberTest = numberCounter;
                numberCounter = 0;
                return BlackHole;
            } else
                return randomRecursive();
    }
}


character randArr[10] = {[0 ... 9] = NormalStep};
bool foundRandom;
uint32_t maxNumTest = 0;

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

    randArr[n].type = randomRecursive();

    /* if (numberTest > maxNumTest)
         maxNumTest = numberTest;
     uartTransmit("recursiveRandom Run number: %lu\n", maxNumTest);*/

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
                *getCharacter(i) = (character) {Air, j, 0, false};
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
        getCharacter(i)->y += shiftStep;
    }
    generateCharacters();
}