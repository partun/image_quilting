#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

int min_err_opt_9;
double tolerance_9 = 1.1;

void calc_errors_corner_opt_9(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap
) {
    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    min_err_opt_9 = INT_MAX;
    int threshold = INT_MAX;

    const int UNROOL = 16;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
            int error = 0;

            // red
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = overlap; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            // green
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = overlap; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            //blue
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = overlap; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }


            if (error < min_err_opt_9) {
                min_err_opt_9 = error;
                threshold = (int) (min_err_opt_9 * tolerance_9);
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}

void calc_errors_above_opt_9(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap
) {
    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    min_err_opt_9 = INT_MAX;
    int threshold = INT_MAX;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
            int error = 0;


            // error on above overlap
            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }


            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = 0; block_y < overlap; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < block_size; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }


            if (error < min_err_opt_9) {
                min_err_opt_9 = error;
                threshold = (int) (min_err_opt_9 * tolerance_9);
            }


            errors[err_idx] = error;
            err_idx++;
        }
    }
}


void calc_errors_left_opt_9(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap
) {
    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int out_x = out_coord.x;
    int out_y = out_coord.y;
    int src_width = src->width;
    int out_width = out->width;

    unsigned char *src_data_r = src->r_data;
    unsigned char *src_data_g = src->g_data;
    unsigned char *src_data_b = src->b_data;
    unsigned char *out_data_r = out->r_data;
    unsigned char *out_data_g = out->g_data;
    unsigned char *out_data_b = out->b_data;

    int diff_0;

    min_err_opt_9 = INT_MAX;
    int threshold = INT_MAX;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
            int error = 0;


            // error on left overlap
            for (int block_y = 0; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_r[src_idx] - out_data_r[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = 0; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_g[src_idx] - out_data_g[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }

            if (error > threshold) {
                errors[err_idx] = INT_MAX;
                err_idx++;
                continue;
            }

            for (int block_y = 0; block_y < block_size; ++block_y) {
                src_idx = (block_y + candidate_y) * src_width + candidate_x;
                out_idx = (block_y + out_y) * out_width + out_x;
                for (int block_x = 0; block_x < overlap; ++block_x) {
                    diff_0 = src_data_b[src_idx] - out_data_b[out_idx];
                    diff_0 = diff_0 * diff_0;
                    error += diff_0;
                    out_idx++;
                    src_idx++;
                }

                if (error > threshold) {
                    break;
                }
            }


            if (error < min_err_opt_9) {
                min_err_opt_9 = error;
                threshold = (int) (min_err_opt_9 * tolerance_9);
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}


//int min_err_opt_9;
/*
 * computes the overlap error for all possible src image blocks
 */
void calc_errors_opt_9(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
) {
    switch (direction) {
        case CORNER:
            calc_errors_corner_opt_9(errors, src, out, out_coord, block_size, overlap);
            break;
        case ABOVE:
            calc_errors_above_opt_9(errors, src, out, out_coord, block_size, overlap);
            break;
        case LEFT:
            calc_errors_left_opt_9(errors, src, out, out_coord, block_size, overlap);
            break;
        default:
            exit(32);
    }
}


//void calc_errors_opt_9(
//        int *errors,
//        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
//        int overlap, Direction direction
//) {
//
//    int src_idx;
//    int out_idx;
//
//    int max_src_x = src->width - block_size;
//    int max_src_y = src->height - block_size;
//
//    ColorV *src_data_r = src->r_data;
//    ColorV *src_data_g = src->g_data;
//    ColorV *src_data_b = src->b_data;
//    ColorV *out_data_r = out->r_data;
//    ColorV *out_data_g = out->g_data;
//    ColorV *out_data_b = out->b_data;
//
//    int r_error, g_error, b_error;
//
//    min_err_opt_9 = INT_MAX;
//
//
//    /* RED */
//    int err_idx = 0;
//    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
//        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
//
//
//            int error = 0;
//            if (direction == ABOVE) {
//                // error on above overlap
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//
//            } else if (direction == LEFT) {
//                // error on left overlap
//                for (int block_y = 0; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//            } else if (direction == CORNER) {
//
//                // red
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//
//                for (int block_y = overlap; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        r_error = src_data_r[src_idx] - out_data_r[out_idx];
//                        r_error = r_error * r_error;
//                        error += r_error;
//                    }
//                }
//            }
//
//            errors[err_idx] = error;
//            err_idx++;
//        }
//    }
//
//    /* GREEN */
//    err_idx = 0;
//    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
//        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
//
//
//            int error = 0;
//            if (direction == ABOVE) {
//                // error on above overlap
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//
//            } else if (direction == LEFT) {
//                // error on left overlap
//                for (int block_y = 0; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//            } else if (direction == CORNER) {
//                // green
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//                for (int block_y = overlap; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        g_error = src_data_g[src_idx] - out_data_g[out_idx];
//                        g_error = g_error * g_error;
//                        error += g_error;
//                    }
//                }
//            }
//            errors[err_idx] += error;
//            err_idx++;
//        }
//    }
//
//    /* BLUE */
//    err_idx = 0;
//    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
//        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {
//
//
//            int error = 0;
//            if (direction == ABOVE) {
//                // error on above overlap
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//
//            } else if (direction == LEFT) {
//                // error on left overlap
//                for (int block_y = 0; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//            } else if (direction == CORNER) {
//                //blue
//                for (int block_y = 0; block_y < overlap; ++block_y) {
//                    for (int block_x = 0; block_x < block_size; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//                for (int block_y = overlap; block_y < block_size; ++block_y) {
//                    for (int block_x = 0; block_x < overlap; ++block_x) {
//                        src_idx = (block_y + candidate_y) * src->width + block_x + candidate_x;
//                        out_idx = (block_y + out_coord.y) * out->width + block_x + out_coord.x;
//
//
//                        b_error = src_data_b[src_idx] - out_data_b[out_idx];
//                        b_error = b_error * b_error;
//                        error += b_error;
//                    }
//                }
//            }
//
//
//            errors[err_idx] += error;
//            if (errors[err_idx] < min_err_opt_9) {
//                min_err_opt_9 = errors[err_idx];
//            }
//            err_idx++;
//        }
//    }
//}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block_opt_9(
        int *errors, ImageRGB *src, int block_size
) {
    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int threshold = (int) (min_err_opt_9 * tolerance_9);
    int candidate_idx = 0;

    for (int i = 0; i < max_src_x * max_src_y; ++i) {
        if (errors[i] <= threshold) {
            errors[candidate_idx] = i;
            candidate_idx++;
        }
    }

    int best_block_idx = errors[rand() % candidate_idx];
    ImageCoordinates best_block_coord = {
            best_block_idx % max_src_x, best_block_idx / max_src_x
    };
    return best_block_coord;
}


