#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

#include <stddef.h>

typedef struct {
    int rows;
    int cols;
    size_t nnz;
    float* values;
    int* row_indices;
    int* col_ptr;
    size_t col_ptr_length;
} csc_matrix;

#endif  // CSC_MATRIX_H