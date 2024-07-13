#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

#include <stddef.h>

typedef struct {
    size_t length;
    int* data;
} intvec;

typedef struct {
    size_t length;
    float* data;
} floatvec;

typedef struct {
    int rows;
    int cols;
    size_t nnz;
    floatvec values;
    intvec row_indices;
    intvec col_ptr;
} csc_matrix;

#endif  // CSC_MATRIX_H