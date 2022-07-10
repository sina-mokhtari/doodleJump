//
// Created by Sina on 6/28/2022.
//

#ifndef DOODLEJUMP_7SEGMENT_H
#define DOODLEJUMP_7SEGMENT_H

#include "requisite.h"

void decimalToBcd(uint32_t n, uint32_t dp, uint16_t dx);

void displayOneFrame(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

void update7Segment(uint32_t s, uint32_t d);

#endif //DOODLEJUMP_7SEGMENT_H
