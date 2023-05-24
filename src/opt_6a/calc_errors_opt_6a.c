#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <immintrin.h>

//void print_128v(__m128i a) {
//    char out[16];
//    _mm_store_si128((__m128i *) &out, a);
//    for (int i = 0; i < 16; ++i) {
//        printf("%d ", out[i]);
//    }
//    printf("\n");
//}
//
//void print_256v32(__m256i a) {
//    int out[8];
//    _mm256_storeu_si256((__m256i *) &out, a);
//    for (int i = 0; i < 8; ++i) {
//        printf("%d ", out[i]);
//    }
//    printf("\n");
//}
//
//void print_256v16(__m256i a) {
//    short out[16];
//    _mm256_storeu_si256((__m256i *) &out, a);
//    for (int i = 0; i < 16; ++i) {
//        printf("%d ", out[i]);
//    }
//    printf("\n");
//}

//
//void test() {
//    __m256i a = _mm256_set_epi16(-1, -2, -3, -4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17);
//    __m256i b = _mm256_set1_epi16(2);
//
//    __m256i result_lo = _mm256_mullo_epi16(a, b);
//    __m256i result_hi = _mm256_mulhi_epi16(a, b);
//
//    __m256i result1 = _mm256_unpackhi_epi16(result_lo, result_hi);
//    __m256i result2 = _mm256_unpacklo_epi16(result_lo, result_hi);
//
//
//    print_256v16(result_lo);
//    print_256v32(result1);
//    print_256v32(result2);
//
//    exit(0);
//}
//



int min_err_opt_6a;

//int min_err_opt_6a;
/*
 * computes the overlap error for all possible src image blocks
 */
void calc_errors_opt_6a(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
) {

    int src_idx;
    int out_idx;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    unsigned char *src_r_data = src->r_data;
    unsigned char *src_g_data = src->g_data;
    unsigned char *src_b_data = src->b_data;

    unsigned char *out_r_data = out->r_data;
    unsigned char *out_g_data = out->g_data;
    unsigned char *out_b_data = out->b_data;

    int r_error, g_error, b_error;

    min_err_opt_6a = INT_MAX;


    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {


            __m256i err_0 = _mm256_set1_epi32(0);
            __m256i err_1 = err_0;
            __m256i err_2 = err_0;
            __m256i err_3 = err_0;
            __m256i err_4 = err_0;
            __m256i err_5 = err_0;
            int error = 0;

            if (direction == ABOVE || direction == CORNER) {
                // error on above overlap

                for (int block_y = 0; block_y < overlap; ++block_y) {
                    src_idx = (block_y + candidate_y) * src->width + candidate_x;
                    out_idx = (block_y + out_coord.y) * out->width + out_coord.x;

                    int block_x = 0;

                    for (; block_x < block_size - 15; block_x += 16) {
                        __m128i src_r = _mm_loadu_si128((const __m128i *) (src_r_data + src_idx));
                        __m128i src_g = _mm_loadu_si128((const __m128i *) (src_g_data + src_idx));
                        __m128i src_b = _mm_loadu_si128((const __m128i *) (src_b_data + src_idx));

                        __m128i out_r = _mm_loadu_si128((const __m128i *) (out_r_data + out_idx));
                        __m128i out_g = _mm_loadu_si128((const __m128i *) (out_g_data + out_idx));
                        __m128i out_b = _mm_loadu_si128((const __m128i *) (out_b_data + out_idx));

                        __m256i src_r_i16 = _mm256_cvtepu8_epi16(src_r);
                        __m256i src_g_i16 = _mm256_cvtepu8_epi16(src_g);
                        __m256i src_b_i16 = _mm256_cvtepu8_epi16(src_b);

                        __m256i out_r_i16 = _mm256_cvtepu8_epi16(out_r);
                        __m256i out_g_i16 = _mm256_cvtepu8_epi16(out_g);
                        __m256i out_b_i16 = _mm256_cvtepu8_epi16(out_b);

                        src_r_i16 = _mm256_sub_epi16(src_r_i16, out_r_i16);
                        src_g_i16 = _mm256_sub_epi16(src_g_i16, out_g_i16);
                        src_b_i16 = _mm256_sub_epi16(src_b_i16, out_b_i16);

                        __m256i sq_err_r_lo = _mm256_mullo_epi16(src_r_i16, src_r_i16);
                        __m256i sq_err_r_hi = _mm256_mulhi_epi16(src_r_i16, src_r_i16);

                        __m256i sq_err_g_lo = _mm256_mullo_epi16(src_g_i16, src_g_i16);
                        __m256i sq_err_g_hi = _mm256_mulhi_epi16(src_g_i16, src_g_i16);

                        __m256i sq_err_b_lo = _mm256_mullo_epi16(src_b_i16, src_b_i16);
                        __m256i sq_err_b_hi = _mm256_mulhi_epi16(src_b_i16, src_b_i16);


                        __m256i res_r_0 = _mm256_unpackhi_epi16(sq_err_r_lo, sq_err_r_hi);
                        __m256i res_r_1 = _mm256_unpacklo_epi16(sq_err_r_lo, sq_err_r_hi);

                        __m256i res_g_0 = _mm256_unpackhi_epi16(sq_err_g_lo, sq_err_g_hi);
                        __m256i res_g_1 = _mm256_unpacklo_epi16(sq_err_g_lo, sq_err_g_hi);

                        __m256i res_b_0 = _mm256_unpackhi_epi16(sq_err_b_lo, sq_err_b_hi);
                        __m256i res_b_1 = _mm256_unpacklo_epi16(sq_err_b_lo, sq_err_b_hi);

                        err_0 = _mm256_add_epi32(err_0, res_r_0);
                        err_1 = _mm256_add_epi32(err_1, res_r_1);

                        err_2 = _mm256_add_epi32(err_2, res_g_0);
                        err_3 = _mm256_add_epi32(err_3, res_g_1);

                        err_4 = _mm256_add_epi32(err_4, res_b_0);
                        err_5 = _mm256_add_epi32(err_5, res_b_1);

                        src_idx += 16;
                        out_idx += 16;
                    }

                    for (; block_x < block_size; ++block_x) {
                        r_error = src_r_data[src_idx] - out_r_data[out_idx];
                        g_error = src_g_data[src_idx] - out_g_data[out_idx];
                        b_error = src_b_data[src_idx] - out_b_data[out_idx];

                        r_error = r_error * r_error;
                        g_error = g_error * g_error;
                        b_error = b_error * b_error;

                        error += r_error + g_error + b_error;

                        src_idx += 1;
                        out_idx += 1;
                    }
                }
            }


            if (direction == LEFT) {
                // error on left overlap

                for (int block_y = 0; block_y < block_size; ++block_y) {
                    src_idx = (block_y + candidate_y) * src->width + candidate_x;
                    out_idx = (block_y + out_coord.y) * out->width + out_coord.x;

                    int block_x = 0;

                    for (; block_x < overlap - 15; block_x += 16) {
                        __m128i src_r = _mm_loadu_si128((const __m128i *) (src_r_data + src_idx));
                        __m128i src_g = _mm_loadu_si128((const __m128i *) (src_g_data + src_idx));
                        __m128i src_b = _mm_loadu_si128((const __m128i *) (src_b_data + src_idx));

                        __m128i out_r = _mm_loadu_si128((const __m128i *) (out_r_data + out_idx));
                        __m128i out_g = _mm_loadu_si128((const __m128i *) (out_g_data + out_idx));
                        __m128i out_b = _mm_loadu_si128((const __m128i *) (out_b_data + out_idx));

                        __m256i src_r_i16 = _mm256_cvtepu8_epi16(src_r);
                        __m256i src_g_i16 = _mm256_cvtepu8_epi16(src_g);
                        __m256i src_b_i16 = _mm256_cvtepu8_epi16(src_b);

                        __m256i out_r_i16 = _mm256_cvtepu8_epi16(out_r);
                        __m256i out_g_i16 = _mm256_cvtepu8_epi16(out_g);
                        __m256i out_b_i16 = _mm256_cvtepu8_epi16(out_b);

                        src_r_i16 = _mm256_sub_epi16(src_r_i16, out_r_i16);
                        src_g_i16 = _mm256_sub_epi16(src_g_i16, out_g_i16);
                        src_b_i16 = _mm256_sub_epi16(src_b_i16, out_b_i16);

                        __m256i sq_err_r_lo = _mm256_mullo_epi16(src_r_i16, src_r_i16);
                        __m256i sq_err_r_hi = _mm256_mulhi_epi16(src_r_i16, src_r_i16);

                        __m256i sq_err_g_lo = _mm256_mullo_epi16(src_g_i16, src_g_i16);
                        __m256i sq_err_g_hi = _mm256_mulhi_epi16(src_g_i16, src_g_i16);

                        __m256i sq_err_b_lo = _mm256_mullo_epi16(src_b_i16, src_b_i16);
                        __m256i sq_err_b_hi = _mm256_mulhi_epi16(src_b_i16, src_b_i16);


                        __m256i res_r_0 = _mm256_unpackhi_epi16(sq_err_r_lo, sq_err_r_hi);
                        __m256i res_r_1 = _mm256_unpacklo_epi16(sq_err_r_lo, sq_err_r_hi);

                        __m256i res_g_0 = _mm256_unpackhi_epi16(sq_err_g_lo, sq_err_g_hi);
                        __m256i res_g_1 = _mm256_unpacklo_epi16(sq_err_g_lo, sq_err_g_hi);

                        __m256i res_b_0 = _mm256_unpackhi_epi16(sq_err_b_lo, sq_err_b_hi);
                        __m256i res_b_1 = _mm256_unpacklo_epi16(sq_err_b_lo, sq_err_b_hi);

                        err_0 = _mm256_add_epi32(err_0, res_r_0);
                        err_1 = _mm256_add_epi32(err_1, res_r_1);

                        err_2 = _mm256_add_epi32(err_2, res_g_0);
                        err_3 = _mm256_add_epi32(err_3, res_g_1);

                        err_4 = _mm256_add_epi32(err_4, res_b_0);
                        err_5 = _mm256_add_epi32(err_5, res_b_1);

                        src_idx += 16;
                        out_idx += 16;
                    }

                    for (; block_x < overlap; ++block_x) {
                        r_error = src_r_data[src_idx] - out_r_data[out_idx];
                        g_error = src_g_data[src_idx] - out_g_data[out_idx];
                        b_error = src_b_data[src_idx] - out_b_data[out_idx];

                        r_error = r_error * r_error;
                        g_error = g_error * g_error;
                        b_error = b_error * b_error;

                        error += r_error + g_error + b_error;

                        src_idx += 1;
                        out_idx += 1;
                    }
                }
            }


            if (direction == CORNER) {
                // error left overlap without the section overlapping with the


                for (int block_y = overlap; block_y < block_size; ++block_y) {
                    src_idx = (block_y + candidate_y) * src->width + candidate_x;
                    out_idx = (block_y + out_coord.y) * out->width + out_coord.x;

                    int block_x = 0;

                    for (; block_x < overlap - 15; block_x += 16) {
                        __m128i src_r = _mm_loadu_si128((const __m128i *) (src_r_data + src_idx));
                        __m128i src_g = _mm_loadu_si128((const __m128i *) (src_g_data + src_idx));
                        __m128i src_b = _mm_loadu_si128((const __m128i *) (src_b_data + src_idx));

                        __m128i out_r = _mm_loadu_si128((const __m128i *) (out_r_data + out_idx));
                        __m128i out_g = _mm_loadu_si128((const __m128i *) (out_g_data + out_idx));
                        __m128i out_b = _mm_loadu_si128((const __m128i *) (out_b_data + out_idx));

                        __m256i src_r_i16 = _mm256_cvtepu8_epi16(src_r);
                        __m256i src_g_i16 = _mm256_cvtepu8_epi16(src_g);
                        __m256i src_b_i16 = _mm256_cvtepu8_epi16(src_b);

                        __m256i out_r_i16 = _mm256_cvtepu8_epi16(out_r);
                        __m256i out_g_i16 = _mm256_cvtepu8_epi16(out_g);
                        __m256i out_b_i16 = _mm256_cvtepu8_epi16(out_b);

                        src_r_i16 = _mm256_sub_epi16(src_r_i16, out_r_i16);
                        src_g_i16 = _mm256_sub_epi16(src_g_i16, out_g_i16);
                        src_b_i16 = _mm256_sub_epi16(src_b_i16, out_b_i16);

                        __m256i sq_err_r_lo = _mm256_mullo_epi16(src_r_i16, src_r_i16);
                        __m256i sq_err_r_hi = _mm256_mulhi_epi16(src_r_i16, src_r_i16);

                        __m256i sq_err_g_lo = _mm256_mullo_epi16(src_g_i16, src_g_i16);
                        __m256i sq_err_g_hi = _mm256_mulhi_epi16(src_g_i16, src_g_i16);

                        __m256i sq_err_b_lo = _mm256_mullo_epi16(src_b_i16, src_b_i16);
                        __m256i sq_err_b_hi = _mm256_mulhi_epi16(src_b_i16, src_b_i16);


                        __m256i res_r_0 = _mm256_unpackhi_epi16(sq_err_r_lo, sq_err_r_hi);
                        __m256i res_r_1 = _mm256_unpacklo_epi16(sq_err_r_lo, sq_err_r_hi);

                        __m256i res_g_0 = _mm256_unpackhi_epi16(sq_err_g_lo, sq_err_g_hi);
                        __m256i res_g_1 = _mm256_unpacklo_epi16(sq_err_g_lo, sq_err_g_hi);

                        __m256i res_b_0 = _mm256_unpackhi_epi16(sq_err_b_lo, sq_err_b_hi);
                        __m256i res_b_1 = _mm256_unpacklo_epi16(sq_err_b_lo, sq_err_b_hi);

                        err_0 = _mm256_add_epi32(err_0, res_r_0);
                        err_1 = _mm256_add_epi32(err_1, res_r_1);

                        err_2 = _mm256_add_epi32(err_2, res_g_0);
                        err_3 = _mm256_add_epi32(err_3, res_g_1);

                        err_4 = _mm256_add_epi32(err_4, res_b_0);
                        err_5 = _mm256_add_epi32(err_5, res_b_1);

                        src_idx += 16;
                        out_idx += 16;
                    }

                    for (; block_x < overlap; ++block_x) {
                        r_error = src_r_data[src_idx] - out_r_data[out_idx];
                        g_error = src_g_data[src_idx] - out_g_data[out_idx];
                        b_error = src_b_data[src_idx] - out_b_data[out_idx];

                        r_error = r_error * r_error;
                        g_error = g_error * g_error;
                        b_error = b_error * b_error;

                        error += r_error + g_error + b_error;

                        src_idx += 1;
                        out_idx += 1;
                    }
                }
            }


            err_0 = _mm256_add_epi32(err_0, err_1);
            err_2 = _mm256_add_epi32(err_2, err_3);
            err_4 = _mm256_add_epi32(err_4, err_5);

            err_0 = _mm256_add_epi32(err_0, err_2);
            err_0 = _mm256_add_epi32(err_0, err_4);

            err_0 = _mm256_hadd_epi32(err_0, err_0);
            err_0 = _mm256_hadd_epi32(err_0, err_0);

            error += _mm256_extract_epi32(err_0, 0);
            error += _mm256_extract_epi32(err_0, 4);


            if (error < min_err_opt_6a) {
                min_err_opt_6a = error;
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block_opt_6a(
        int *errors, ImageRGB *src, int block_size
) {
    double tolerance = 1.1;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int threshold = (int) (min_err_opt_6a * tolerance);
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


