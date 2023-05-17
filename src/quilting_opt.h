#ifndef QUILTING_OPT_H
#define QUILTING_OPT_H

#include "load_image.h"
#include "calc_errors_opt.h"

Image *quilting_opt(Image *image, int block_size, int out_num_blocks, int overlap_size);

#endif //QUILTING_OPT_H
