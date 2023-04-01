//
// Created by Oleh Kuzyk on 01.04.23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "calc_errors.h"

void test_calc_errors() {
    // Create test image
    RGB img_data[] = {
        { 20,  40,  60}, { 80, 100, 120}, {140, 160, 180}, {200, 220, 240},
        { 15,  35,  55}, { 75,  95, 115}, {135, 155, 175}, {195, 215, 235},
        { 10,  30,  50}, { 70,  90, 110}, {130, 150, 170}, {190, 210, 230},
        {  5,  25,  45}, { 65,  85, 105}, {125, 145, 165}, {185, 205, 225},
    };
    Image img = {4, 4, img_data};

    // Create output slice
    RGB slice_data[] = {
        { 20,  40,  60}, { 80, 100, 120},
        { 15,  35,  55}, { 75,  95, 115},
    };
    Image out_slice = {2, 2, slice_data};

    // Calculate errors
    double* errors = calc_errors(&img, &out_slice, 2, 2, "above");

    assert(sizeof(errors) == 4);

    // Check errors
    assert(errors[0] == 0.0);
    assert(errors[1] == 43200.0);
    assert(errors[2] == 300.0);
    assert(errors[3] == 36300.0);

    printf("Error calculation tests passed successfully!\n");

    free(errors);
}
