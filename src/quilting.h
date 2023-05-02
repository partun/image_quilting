#ifndef QUILTING_H
#define QUILTING_H

#include "load_image.h"
#include "calc_errors.h"


Image *quilting(Image *image, int block_size, int out_num_blocks, int overlap_size);


#endif //QUILTING_H
