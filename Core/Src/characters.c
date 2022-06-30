//
// Created by Sina on 6/30/2022.
//

#include "characters.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

uint32_t probabilities[2];

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

Character characters[82];
//[2 ... 79] = {(characterType) NULL, 0, 0}

int replaceRand;
int replaceRand2;

void charactersInit() {
    for (int i = 2, y = -1, j; i < 82; i++) {
        j = (i-2) % 4;
        characters[i] = (Character) {AIR, j, (j != 0) ? y : ++y};
    }
    characters[0] = (Character) {DOODLER_UP, 2, 9};
    characters[1] = (Character) {DOODLER_DOWN, 2, 10};

    //characters[56] = (Character) {NORMAL_STEP, 2, 13};
    characters[20] = (Character) {NORMAL_STEP, 2, 4};
    characters[40] = (Character) {NORMAL_STEP, 2, 9};
    characters[56] = (Character) {NORMAL_STEP, 2, 13};
}

void replaceNewCharacters(uint32_t n) {
    switch (n) {
        case 0:
            for (int i = 2, j = 0; j < 4 && i < 82; i++) {
                if (characters[i].y == 20) {
                    characters[i] = (Character) {AIR, j++, 0};
                }
            }
            break;
        case 1:
            replaceRand = rand() % 4;
            for (int i = 2, j = 0; j < 4 && i < 82; i++) {
                if (characters[i].y == 20) {
                    characters[i] = (Character) {(j != replaceRand) ? AIR : NORMAL_STEP, j++, 0};
                }
            }
            break;
        default: // 2
            replaceRand = rand() % 4;
            replaceRand2 = rand() % 4;
            for (int i = 2, j = 0; j < 4 && i < 82; i++) {
                if (characters[i].y == 20) {
                    characters[i] = (Character) {(j == replaceRand || j == replaceRand2) ? NORMAL_STEP : AIR,
                                                 j++, 0};
                }
            }
            break;
    }

}

bool generate;
Character generatedCharacters[4];

void generateCharacters() {
    if ((rand() % 25) < probabilities[1]) // generate
        if ((rand() % 25) < probabilities[1]) // generate 2
            replaceNewCharacters(2);
        else    // generate 1
            replaceNewCharacters(1);

    else    // not generate
        replaceNewCharacters(0);
}

void shiftDownCharacters(uint32_t shiftStep) {
    for (int i = 2; i < 82; i++) {
        if (characters[i].type != BULLET)
            characters[i].y += shiftStep;

    }
    generateCharacters();
}

