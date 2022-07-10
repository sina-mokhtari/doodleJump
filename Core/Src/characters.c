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

byte doodlerUpDizzyByte[] = {
        0x0F,
        0x18,
        0x15,
        0x12,
        0x15,
        0x18,
        0x0F,
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

character charactersArr2[20][4];
doodlerType doodler;
character bullets[BULLETS_BUFFER_SIZE];

bool collisionWithDoodler(uint32_t x, uint32_t y) {
    return (x == doodler.upper.x && y == doodler.upper.y) ||
           (x == doodler.lower.x && y == doodler.lower.y);
}

void gameCharactersInit() {

    for (int i = 0; i < VERTICAL_LCD_ROWS; i++) {
        for (int j = 0; j < VERTICAL_LCD_COLUMNS; j++) {
            *getCharacter(j, i) = (character) {Air, j, i, false};
        }
    }

    doodler.dizzy = false;
    doodler.upper = (character) {DoodlerUp, 2, 9, false};
    doodler.lower = (character) {DoodlerDown, 2, 10, false};

    for (int i = 0; i < BULLETS_BUFFER_SIZE; ++i) {
        bullets[i].characterFlag = false;
    }

    *getCharacter(2, 4) = (character) {SpringStep, 2, 4, false};
    *getCharacter(2, 9) = (character) {NormalStep, 2, 9, false};
    *getCharacter(2, 13) = (character) {NormalStep, 2, 13, false};
    *lcdArr(3, 19) = Bullet;
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
     uartFormatTransmit("recursiveRandom Run number: %lu\n", maxNumTest);*/

    for (int i = 0; i < 4; ++i) {
        foundRandom = false;
        for (int l = 0; l <= n; l++) {
            if (i == randArr[l].x) {
                *getCharacter(i, 0) = (character) {randArr[l].type, i, 0, false};
                foundRandom = true;
                break;
            }
        }
        if (!foundRandom)
            *getCharacter(i, 0) = (character) {Air, i, 0, false};
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

void shiftDownCharacters() {
    for (int i = 18; i >= 0; i--) {
        for (int j = 3; j >= 0; j--) {
            *getCharacter(j, i + 1) = *getCharacter(j, i);
            getCharacter(j, i + 1)->y++;
        }
    }
    generateCharacters();
}

char *characterTypeHash(characterType characterArg) {
    switch (characterArg) {
        case Air:
            return "20";
            break;
        case DoodlerUp:
            return "00";
            break;
        case DoodlerDown:
            return "01";
            break;
        case NormalStep:
            return "02";
            break;
        case BrokenStep:
            return "03";
            break;
        case SpringStep:
            return "04";
            break;
        case Monster:
            return "05";
            break;
        case BlackHole:
            return "06";
            break;
        case Bullet:
            return "a5";
            break;
        default:
            break;
    }
}

char myTmpChar[5];

char *characterHash(character characterArg) {
    switch (characterArg.type) {
        case Air:
            if (characterArg.characterFlag)
                return "201";
            else
                return "200";
            break;
        case DoodlerUp:
            if (characterArg.characterFlag)
                return "001";
            else
                return "000";
            break;
        case DoodlerDown:
            if (characterArg.characterFlag)
                return "011";
            else
                return "010";
            break;
        case NormalStep:
            if (characterArg.characterFlag)
                return "021";
            else
                return "020";
            break;
        case BrokenStep:
            if (characterArg.characterFlag)
                return "031";
            else
                return "030";
            break;
        case SpringStep:
            if (characterArg.characterFlag)
                return "041";
            else
                return "040";
            break;
        case Monster:
            if (characterArg.characterFlag)
                return "051";
            else
                return "050";
            break;
        case BlackHole:
            if (characterArg.characterFlag)
                return "061";
            else
                return "060";
            break;
        case Bullet:
            if (characterArg.characterFlag)
                return "a51";
            else
                return "a50";
            break;
        default:
            break;
    }
}