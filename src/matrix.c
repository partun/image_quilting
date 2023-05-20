#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

void free_matrix(Matrix *m) {
    free(m->data);
    free(m);
}

void print_matrix(Matrix *m) {
    int width = m->width;
    int height = m->height;

    printf("\n");
    for (int i = 0; i < height; ++i) {
        printf("%2d:", i);
        for (int j = 0; j < width; ++j) {
            int x = m->data[i * width + j];
            printf(" %5d ", x);
        }
        printf("\n");
    }
    printf("\n");
}

int matrix_equal_size(Matrix *a, Matrix *b) {
    return a->width == b->width || a->height == b->height;
}

int matrix_equal(Matrix *a, Matrix *b) {
    if (a->width != b->width || a->height != b->height) {
        return 0;
    }
    int height = a->height;
    int width = b->width;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (a->data[i * width + j] != b->data[i * width + j]) {
                return 0;
            }
        }
    }
    return 1;
}

Matrix *transpose(Matrix *a) {
    Matrix *a_transposed = (Matrix *) malloc(sizeof(Matrix));
    a_transposed->width = a->height;
    a_transposed->height = a->width;
    a_transposed->data = (int *) malloc(a->height * a->width * sizeof(int));

    int transposed_idx = 0;
    for (int j = 0; j < a->width; ++j) {
        for (int i = 0; i < a->height; ++i) {
            a_transposed->data[transposed_idx] = a->data[i * a->width + j];
            transposed_idx++;
        }
    }

    free_matrix(a);
    return a_transposed;
}