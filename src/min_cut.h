#ifndef TEAM08_MIN_CUT_H
#define TEAM08_MIN_CUT_H

#include "calc_errors.h"
#include "matrix.h"
#include "image.h"


Matrix *calc_overlap_error(Image *source_image, Image *output_image, ImageCoordinates block_coords,
                           ImageCoordinates output_coords,
                           int overlap_width, int overlap_height);

Matrix *min_cut(
        Image *source_image, Image *output_image, ImageCoordinates block_coords, ImageCoordinates output_coords,
        int block_size, int overlap, Direction direction
);

#endif //TEAM08_MIN_CUT_H
