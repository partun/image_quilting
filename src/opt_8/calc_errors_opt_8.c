#include "src/matrix.h"
#include "src/image.h"
#include <stdlib.h>
#include <limits.h>
#include <immintrin.h>

#include <stdio.h>

#define UNROLL 16

int min_err_opt_8;

//int min_err_opt_8;
/*
 * computes the overlap error for all possible src image blocks
 */
void calc_errors_opt_8(
        int *errors,
        ImageRGB *src, ImageRGB *out, ImageCoordinates out_coord, int block_size,
        int overlap, Direction direction
) {

    int src_idx;
    int out_idx;
    int src_idx_off;
    int out_idx_off;
    int err_idx = 0;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;

    int r_error, g_error, b_error;

    min_err_opt_8 = INT_MAX;

    //
    ColorV *src_r_data = src->r_data;
    ColorV *src_g_data = src->g_data;
    ColorV *src_b_data = src->b_data;

    ColorV *out_r_data = out->r_data;
    ColorV *out_g_data = out->g_data;
    ColorV *out_b_data = out->b_data;
    //
    for (int candidate_y = 0; candidate_y < max_src_y; ++candidate_y) {
        for (int candidate_x = 0; candidate_x < max_src_x; ++candidate_x) {


            int error = 0;

            int error0 = 0;
            int error1 = 0;
            int error2 = 0;

            if (direction == LEFT) {
                // error on left overlap
                
                src_idx = (candidate_y) * src->width + candidate_x;
                out_idx = (out_coord.y) * out->width + out_coord.x;
                src_idx_off = src->width-overlap;
                out_idx_off = out->width-overlap;
                
                __m256i err_0 = _mm256_set1_epi32(0);
                __m256i err_1 = _mm256_set1_epi32(0);
               

                for (int block_y = 0; block_y < block_size; ++block_y) {
                    int block_x = 0;
                    for (; block_x < overlap-UNROLL+1; block_x+=UNROLL) {
  
                    __m128i r_src = _mm_loadu_si128((const __m128i_u *) (src_r_data + src_idx));
                    __m128i g_src = _mm_loadu_si128((const __m128i_u *) (src_g_data + src_idx));
                    __m128i b_src = _mm_loadu_si128((const __m128i_u *) (src_b_data + src_idx));

                    __m128i r_out = _mm_loadu_si128((const __m128i_u *) (out_r_data + out_idx));
                    __m128i g_out = _mm_loadu_si128((const __m128i_u *) (out_g_data + out_idx));
                    __m128i b_out = _mm_loadu_si128((const __m128i_u *) (out_b_data + out_idx));

                    __m256i r_src_int_0 = _mm256_cvtepu8_epi32(r_src);
                    __m256i g_src_int_0 = _mm256_cvtepu8_epi32(g_src);
                    __m256i b_src_int_0 = _mm256_cvtepu8_epi32(b_src);

                    __m256i r_out_int_0 = _mm256_cvtepu8_epi32(r_out);
                    __m256i g_out_int_0 = _mm256_cvtepu8_epi32(g_out);
                    __m256i b_out_int_0 = _mm256_cvtepu8_epi32(b_out);

                    r_src = (__m128i) _mm_permute_pd((__m128d) r_src, 0b01);
                    g_src = (__m128i) _mm_permute_pd((__m128d) g_src, 0b01);
                    b_src = (__m128i) _mm_permute_pd((__m128d) b_src, 0b01);

                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);
                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);
                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);

                    __m256i r_src_int_1 = _mm256_cvtepu8_epi32(r_src);
                    __m256i g_src_int_1 = _mm256_cvtepu8_epi32(g_src);
                    __m256i b_src_int_1 = _mm256_cvtepu8_epi32(b_src);

                    __m256i r_out_int_1 = _mm256_cvtepu8_epi32(r_out);
                    __m256i g_out_int_1 = _mm256_cvtepu8_epi32(g_out);
                    __m256i b_out_int_1 = _mm256_cvtepu8_epi32(b_out);

                    r_src_int_0 = _mm256_sub_epi32(r_src_int_0, r_out_int_0);
                    g_src_int_0 = _mm256_sub_epi32(g_src_int_0, g_out_int_0);
                    b_src_int_0 = _mm256_sub_epi32(b_src_int_0, b_out_int_0);
                 
                    r_src_int_0 = _mm256_mul_epi32(r_src_int_0, r_src_int_0);
                    g_src_int_0 = _mm256_mul_epi32(g_src_int_0, g_src_int_0);
                    b_src_int_0 = _mm256_mul_epi32(b_src_int_0, b_src_int_0);

                    r_src_int_1 = _mm256_sub_epi32(r_src_int_1, r_out_int_1);
                    g_src_int_1 = _mm256_sub_epi32(g_src_int_1, g_out_int_1);
                    b_src_int_1 = _mm256_sub_epi32(b_src_int_1, b_out_int_1);

                    r_src_int_1 = _mm256_mul_epi32(r_src_int_1, r_src_int_1);
                    g_src_int_1 = _mm256_mul_epi32(g_src_int_1, g_src_int_1);
                    b_src_int_1 = _mm256_mul_epi32(b_src_int_1, b_src_int_1);

                    err_0 = _mm256_add_epi32(err_0, r_src_int_0);
                    err_0 = _mm256_add_epi32(err_0, g_src_int_0);                    
                    err_0 = _mm256_add_epi32(err_0, b_src_int_0);

                    err_1 = _mm256_add_epi32(err_1, r_src_int_1);
                    err_1 = _mm256_add_epi32(err_1, g_src_int_1);
                    err_1 = _mm256_add_epi32(err_1, b_src_int_1);

                    src_idx += UNROLL;
                    out_idx += UNROLL;
                    }


                    for (; block_x < overlap; block_x++) {
                    
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];
                    
                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        src_idx++;
                        out_idx++;
                    
                    }
                    src_idx+=src_idx_off;
                    out_idx+=out_idx_off;
                }

                err_0 = _mm256_add_epi32(err_0, err_1);
                __m256i err_red4 = (__m256i) _mm256_permute_pd((__m256d)err_0, 0b1111);
                err_0 = _mm256_add_epi32(err_0, err_red4);
                __m256i err_red2 = (__m256i) _mm256_permute_ps((__m256)err_0, 0b01010101);
                err_0 = _mm256_add_epi32(err_0, err_red2);
                __m256i err_red1 = (__m256i) _mm256_permute4x64_pd((__m256d)err_0, 0b10101010);
                err_0 = _mm256_add_epi32(err_0, err_red1);

                int err_total = _mm256_extract_epi32(err_0,0);
                error0+=err_total;
            }
            if (direction == ABOVE || direction == CORNER) {
                       // error on above overlap
                
                src_idx = (candidate_y) * src->width + candidate_x;
                out_idx = (out_coord.y) * out->width + out_coord.x;
                src_idx_off = src->width-block_size;
                out_idx_off = out->width-block_size;

                __m256i err_0 = _mm256_set1_epi32(0);
                __m256i err_1 = _mm256_set1_epi32(0);
               

                for (int block_y = 0; block_y < overlap; ++block_y) {
                    int block_x = 0;
                    for (; block_x < block_size-UNROLL+1; block_x+=UNROLL) {
  
                    __m128i r_src = _mm_loadu_si128((const __m128i_u *) (src_r_data + src_idx));
                    __m128i g_src = _mm_loadu_si128((const __m128i_u *) (src_g_data + src_idx));
                    __m128i b_src = _mm_loadu_si128((const __m128i_u *) (src_b_data + src_idx));

                    __m128i r_out = _mm_loadu_si128((const __m128i_u *) (out_r_data + out_idx));
                    __m128i g_out = _mm_loadu_si128((const __m128i_u *) (out_g_data + out_idx));
                    __m128i b_out = _mm_loadu_si128((const __m128i_u *) (out_b_data + out_idx));

                    __m256i r_src_int_0 = _mm256_cvtepu8_epi32(r_src);
                    __m256i g_src_int_0 = _mm256_cvtepu8_epi32(g_src);
                    __m256i b_src_int_0 = _mm256_cvtepu8_epi32(b_src);

                    __m256i r_out_int_0 = _mm256_cvtepu8_epi32(r_out);
                    __m256i g_out_int_0 = _mm256_cvtepu8_epi32(g_out);
                    __m256i b_out_int_0 = _mm256_cvtepu8_epi32(b_out);

                    r_src = (__m128i) _mm_permute_pd((__m128d) r_src, 0b01);
                    g_src = (__m128i) _mm_permute_pd((__m128d) g_src, 0b01);
                    b_src = (__m128i) _mm_permute_pd((__m128d) b_src, 0b01);

                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);
                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);
                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);

                    __m256i r_src_int_1 = _mm256_cvtepu8_epi32(r_src);
                    __m256i g_src_int_1 = _mm256_cvtepu8_epi32(g_src);
                    __m256i b_src_int_1 = _mm256_cvtepu8_epi32(b_src);

                    __m256i r_out_int_1 = _mm256_cvtepu8_epi32(r_out);
                    __m256i g_out_int_1 = _mm256_cvtepu8_epi32(g_out);
                    __m256i b_out_int_1 = _mm256_cvtepu8_epi32(b_out);

                    r_src_int_0 = _mm256_sub_epi32(r_src_int_0, r_out_int_0);
                    g_src_int_0 = _mm256_sub_epi32(g_src_int_0, g_out_int_0);
                    b_src_int_0 = _mm256_sub_epi32(b_src_int_0, b_out_int_0);
                 
                    r_src_int_0 = _mm256_mul_epi32(r_src_int_0, r_src_int_0);
                    g_src_int_0 = _mm256_mul_epi32(g_src_int_0, g_src_int_0);
                    b_src_int_0 = _mm256_mul_epi32(b_src_int_0, b_src_int_0);

                    r_src_int_1 = _mm256_sub_epi32(r_src_int_1, r_out_int_1);
                    g_src_int_1 = _mm256_sub_epi32(g_src_int_1, g_out_int_1);
                    b_src_int_1 = _mm256_sub_epi32(b_src_int_1, b_out_int_1);

                    r_src_int_1 = _mm256_mul_epi32(r_src_int_1, r_src_int_1);
                    g_src_int_1 = _mm256_mul_epi32(g_src_int_1, g_src_int_1);
                    b_src_int_1 = _mm256_mul_epi32(b_src_int_1, b_src_int_1);

                    err_0 = _mm256_add_epi32(err_0, r_src_int_0);
                    err_0 = _mm256_add_epi32(err_0, g_src_int_0);                    
                    err_0 = _mm256_add_epi32(err_0, b_src_int_0);

                    err_1 = _mm256_add_epi32(err_1, r_src_int_1);
                    err_1 = _mm256_add_epi32(err_1, g_src_int_1);
                    err_1 = _mm256_add_epi32(err_1, b_src_int_1);

                    src_idx += UNROLL;
                    out_idx += UNROLL;
                    }


                    for (; block_x < block_size; block_x++) {
                    
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];
                    
                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        src_idx++;
                        out_idx++;
                    
                    }
                    src_idx+=src_idx_off;
                    out_idx+=out_idx_off;
                }

                err_0 = _mm256_add_epi32(err_0, err_1);
                __m256i err_red4 = (__m256i) _mm256_permute_pd((__m256d)err_0, 0b1111);
                err_0 = _mm256_add_epi32(err_0, err_red4);
                __m256i err_red2 = (__m256i) _mm256_permute_ps((__m256)err_0, 0b01010101);
                err_0 = _mm256_add_epi32(err_0, err_red2);
                __m256i err_red1 = (__m256i) _mm256_permute4x64_pd((__m256d)err_0, 0b10101010);
                err_0 = _mm256_add_epi32(err_0, err_red1);

                int err_total = _mm256_extract_epi32(err_0,0);
                error0+=err_total;
            }
            if (direction == CORNER) {
                // error left overlap without the section overlapping with the

                src_idx = (candidate_y+overlap) * src->width + candidate_x;
                out_idx = (out_coord.y+overlap) * out->width + out_coord.x;
                src_idx_off = src->width-overlap;
                out_idx_off = out->width-overlap;

                
                __m256i err_0 = _mm256_set1_epi32(0);
                __m256i err_1 = _mm256_set1_epi32(0);
               

                for (int block_y = overlap; block_y < block_size; ++block_y) {
                    int block_x = 0;
                    for (; block_x < overlap-UNROLL+1; block_x+=UNROLL) {
  
                    __m128i r_src = _mm_loadu_si128((const __m128i_u *) (src_r_data + src_idx));
                    __m128i g_src = _mm_loadu_si128((const __m128i_u *) (src_g_data + src_idx));
                    __m128i b_src = _mm_loadu_si128((const __m128i_u *) (src_b_data + src_idx));

                    __m128i r_out = _mm_loadu_si128((const __m128i_u *) (out_r_data + out_idx));
                    __m128i g_out = _mm_loadu_si128((const __m128i_u *) (out_g_data + out_idx));
                    __m128i b_out = _mm_loadu_si128((const __m128i_u *) (out_b_data + out_idx));

                    __m256i r_src_int_0 = _mm256_cvtepu8_epi32(r_src);
                    __m256i g_src_int_0 = _mm256_cvtepu8_epi32(g_src);
                    __m256i b_src_int_0 = _mm256_cvtepu8_epi32(b_src);

                    __m256i r_out_int_0 = _mm256_cvtepu8_epi32(r_out);
                    __m256i g_out_int_0 = _mm256_cvtepu8_epi32(g_out);
                    __m256i b_out_int_0 = _mm256_cvtepu8_epi32(b_out);

                    r_src = (__m128i) _mm_permute_pd((__m128d) r_src, 0b01);
                    g_src = (__m128i) _mm_permute_pd((__m128d) g_src, 0b01);
                    b_src = (__m128i) _mm_permute_pd((__m128d) b_src, 0b01);

                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);
                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);
                    b_out = (__m128i) _mm_permute_pd((__m128d) b_out, 0b01);

                    __m256i r_src_int_1 = _mm256_cvtepu8_epi32(r_src);
                    __m256i g_src_int_1 = _mm256_cvtepu8_epi32(g_src);
                    __m256i b_src_int_1 = _mm256_cvtepu8_epi32(b_src);

                    __m256i r_out_int_1 = _mm256_cvtepu8_epi32(r_out);
                    __m256i g_out_int_1 = _mm256_cvtepu8_epi32(g_out);
                    __m256i b_out_int_1 = _mm256_cvtepu8_epi32(b_out);

                    r_src_int_0 = _mm256_sub_epi32(r_src_int_0, r_out_int_0);
                    g_src_int_0 = _mm256_sub_epi32(g_src_int_0, g_out_int_0);
                    b_src_int_0 = _mm256_sub_epi32(b_src_int_0, b_out_int_0);
                 
                    r_src_int_0 = _mm256_mul_epi32(r_src_int_0, r_src_int_0);
                    g_src_int_0 = _mm256_mul_epi32(g_src_int_0, g_src_int_0);
                    b_src_int_0 = _mm256_mul_epi32(b_src_int_0, b_src_int_0);

                    r_src_int_1 = _mm256_sub_epi32(r_src_int_1, r_out_int_1);
                    g_src_int_1 = _mm256_sub_epi32(g_src_int_1, g_out_int_1);
                    b_src_int_1 = _mm256_sub_epi32(b_src_int_1, b_out_int_1);

                    r_src_int_1 = _mm256_mul_epi32(r_src_int_1, r_src_int_1);
                    g_src_int_1 = _mm256_mul_epi32(g_src_int_1, g_src_int_1);
                    b_src_int_1 = _mm256_mul_epi32(b_src_int_1, b_src_int_1);

                    err_0 = _mm256_add_epi32(err_0, r_src_int_0);
                    err_0 = _mm256_add_epi32(err_0, g_src_int_0);                    
                    err_0 = _mm256_add_epi32(err_0, b_src_int_0);

                    err_1 = _mm256_add_epi32(err_1, r_src_int_1);
                    err_1 = _mm256_add_epi32(err_1, g_src_int_1);
                    err_1 = _mm256_add_epi32(err_1, b_src_int_1);

                    src_idx += UNROLL;
                    out_idx += UNROLL;
                    }


                    for (; block_x < overlap; block_x++) {
                    
                        r_error = src->r_data[src_idx] - out->r_data[out_idx];
                        g_error = src->g_data[src_idx] - out->g_data[out_idx];
                        b_error = src->b_data[src_idx] - out->b_data[out_idx];
                    
                        error0 += r_error * r_error;
                        error1 += g_error * g_error;
                        error2 += b_error * b_error;
                        
                        src_idx++;
                        out_idx++;
                    
                    }
                    src_idx+=src_idx_off;
                    out_idx+=out_idx_off;
                }

                err_0 = _mm256_add_epi32(err_0, err_1);
                __m256i err_red4 = (__m256i) _mm256_permute_pd((__m256d)err_0, 0b1111);
                err_0 = _mm256_add_epi32(err_0, err_red4);
                __m256i err_red2 = (__m256i) _mm256_permute_ps((__m256)err_0, 0b01010101);
                err_0 = _mm256_add_epi32(err_0, err_red2);
                __m256i err_red1 = (__m256i) _mm256_permute4x64_pd((__m256d)err_0, 0b10101010);
                err_0 = _mm256_add_epi32(err_0, err_red1);

                int err_total = _mm256_extract_epi32(err_0,0);

                error0+=err_total;
            }

            error += error0 + error1 + error2;

            if (error < min_err_opt_8) {
                min_err_opt_8 = error;
            }

            errors[err_idx] = error;
            err_idx++;
        }
    }
}

/*
 * chooses a random block from the source image with an error within the tolerance
 */
ImageCoordinates find_best_block_opt_8(
        const int *errors, ImageRGB *src, int block_size,
        ImageCoordinates *candidate_coords
) {
    double tolerance = 1.1;

    int max_src_x = src->width - block_size;
    int max_src_y = src->height - block_size;
/*
    ImageCoordinates *candidate_coords = (ImageCoordinates *) malloc(
            max_src_x * max_src_y * sizeof(ImageCoordinates));
*/    int candidate_idx = 0;

    int threshold = (int) (min_err_opt_8 * tolerance);

    for (int y = 0; y < max_src_y; y++) {
        for (int x = 0; x < max_src_x; x++) {
            if (errors[y * max_src_x + x] <= threshold) {
                ImageCoordinates coord = {x, y};
                candidate_coords[candidate_idx] = coord;
                candidate_idx++;
            }
        }
    }
    // printf("min error = %d, threshold = %d, candidate_idx = %d\n",
    //     min_err, threshold, candidate_idx);

    ImageCoordinates best_block_coord = candidate_coords[rand() % candidate_idx];
    // free(candidate_coords);
    return best_block_coord;
}


