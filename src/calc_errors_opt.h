//
// Created by Oleh Kuzyk on 10.03.23.
//

#ifndef CALC_ERRORS_OPT_H
#define CALC_ERRORS_OPT_H

#include "load_image.h"
#include "calc_errors.h"

int *calc_errors_opt(Image *src, Image *out, ImageCoordinates out_coord, int block_size,
                    int overlap, Direction direction);

ImageCoordinates find_best_block_opt(const int *errors, Image *src, int block_size);

#endif //CALC_ERRORS_OPT_H
