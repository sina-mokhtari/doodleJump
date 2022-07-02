//
// Created by Sina on 6/30/2022.
//

#include "characters.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#define SUM_OF_PROBABILITIES 100

uint32_t probabilities[2];
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

character characters[80];
character doodler[2];
//[2 ... 79] = {(characterType) NULL, 0, 0}

int replaceRand;
int replaceRand2;

void charactersInit() {
    for (int i = 0, y = -1, j; i < 80; i++) {
        j = i % 4;
        characters[i] = (character) {Air, j, (j != 0) ? y : ++y};
    }
    doodler[0] = (character) {DoodlerUp, 2, 9};
    doodler[1] = (character) {DoodlerDown, 2, 10};

    //characters[56] = (character) {NormalStep, 2, 13};
    characters[18] = (character) {SpringStep, 2, 4};
    characters[38] = (character) {NormalStep, 2, 9};
    characters[54] = (character) {NormalStep, 2, 13};
}

characterType randomAdditional() {
   /* for (int i = 0; i < 3; i++) {
        switch (rand() % 4) {
            case 0:
                if (rand() % SUM_OF_PROBABILITIES < brokenStepProbability)
                    return BrokenStep;
                break;
            case 1:
                if (rand() % SUM_OF_PROBABILITIES < springStepProbability)
                    return SpringStep;
                break;
            case 2:
                if (rand() % SUM_OF_PROBABILITIES < monsterProbability)
                    return Monster;
                break;
            default :
                if (rand() % SUM_OF_PROBABILITIES < blackHoleProbability)
                    return BlackHole;
                break;
        }
    }*/

  /*  if (rand() % SUM_OF_PROBABILITIES < brokenStepProbability)
        return BrokenStep;
    else if (rand() % SUM_OF_PROBABILITIES < springStepProbability)
        return SpringStep;
    else if (rand() % SUM_OF_PROBABILITIES < monsterProbability)
        return Monster;
    else if (rand() % SUM_OF_PROBABILITIES < blackHoleProbability)
        return BlackHole;*/
    return Air;
}



character randArr[4];
bool foundRandom;

void replaceNewCharacters(uint32_t n) {

    // this code is the problem
//   for (int i = 0; i < n; i++) {
    //    randArr[i].x = rand() % 4;
//randArr[i].x = i;
       /* for (int j = 0; j < i; j++) {
            if (randArr[i].x == randArr[j].x) {
                i--;
                break;
            }
        }*/
     //   randArr[i].type = NormalStep;
    //}

   /* randArr[n].type = randomAdditional();

    for (int i = 0, j = 0; j < 4 && i < 80; i++) {
        if (characters[i].y == 20) {
              foundRandom = false;
            for (int l = 0; l <= n; l++) {
                if (j == randArr[l].x) {
                    characters[i] = (character) {randArr[l].type, j, 20};
                    foundRandom = true;
                    break;
                }
            }
            if (!foundRandom)
                characters[i] = (character) {Air, j, 20};
            //} else
            // characters[i] = (character) {Air, j, 20};

            j++;
        }
    }
*/
   //-----------------------------
    switch (n) {
        case 0:
            for (int i = 0, j = 0; j < 4 && i < 80; i++) {
                if (characters[i].y == 20) {
                    characters[i] = (character) {Air, j++, 0};
                }
            }
            break;
        case 1:
            replaceRand = rand() % 4;
            for (int i = 0, j = 0; j < 4 && i < 80; i++) {
                if (characters[i].y == 20) {
                    characters[i] = (character) {(j != replaceRand) ? Air : NormalStep, j++, 0};
                }
            }
            break;
        default: // 2
            replaceRand = rand() % 4;
            replaceRand2 = rand() % 4;
            for (int i = 0, j = 0; j < 4 && i < 80; i++) {
                if (characters[i].y == 20) {
                    characters[i] = (character) {(j == replaceRand || j == replaceRand2) ? NormalStep : Air,
                                                 j++, 0};
                }
            }
            break;
    }
}


//bool generate;
character generatedCharacters[4];
uint_fast8_t distanceToLastStep;

void generateCharacters() {

    if ((rand() % SUM_OF_PROBABILITIES) < generateProbability) { // generate
        if ((rand() % SUM_OF_PROBABILITIES) < twoStepProbability) // generate 2 steps
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
    for (int i = 0; i < 80; i++) {
        if (characters[i].type != Bullet)
            characters[i].y += shiftStep;

    }
    generateCharacters();
}

