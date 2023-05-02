//
// Created by Oleh Kuzyk on 10.03.23.
//

#ifndef CALC_ERRORS_H
#define CALC_ERRORS_H

#include "load_image.h"

typedef enum Direction {
    ABOVE,
    LEFT,
    CORNER,
    FIRST
} Direction;

double* calc_errors(Image *image, Image* out_slice, int block_size, int overlap_size, char* direction);

#endif //CALC_ERRORS_H
