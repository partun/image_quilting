#ifndef QUILTING_H
#define QUILTING_H

#include "src/load_image.h"
#include "src/baseline/calc_errors.h"


Image *quilting_baseline(Image *image, int block_size, int out_num_blocks, int overlap_size);


#endif //QUILTING_H
