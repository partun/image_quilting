#ifndef QUILTING_H
#define QUILTING_H

#include "load_image.h"
#include "calc_errors.h"

typedef struct ImageCoordinates {
    unsigned int x;
    unsigned int y;
} ImageCoordinates;


Image *quilting(Image *image, int block_size, int num_blocks, int overlap_size);

Matrix *calc_overlap_error(Image *source_image, Image *output_image, ImageCoordinates block_coords,
                           ImageCoordinates output_coords,
                           int block_size, int overlap, Direction direction);

Matrix *min_cut(
        Image *source_image, Image *output_image, ImageCoordinates block_coords, ImageCoordinates output_coords,
        int block_size, int overlap, Direction direction
);

#endif //QUILTING_H
