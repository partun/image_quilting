#ifndef TEAM08_MATRIX_H
#define TEAM08_MATRIX_H

typedef struct Matrix {
    unsigned int width;
    unsigned int height;
    int *data;
} Matrix;

void free_matrix(Matrix *m);

void print_matrix(Matrix *m);

int matrix_equal_size(Matrix *a, Matrix *b);

int matrix_equal(Matrix *a, Matrix *b);

Matrix *transpose(Matrix *a);

#endif //TEAM08_MATRIX_H
