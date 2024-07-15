#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

#include <stddef.h>

typedef struct {
    size_t rows;
    size_t cols;
    size_t nnz;
    float* values;
    size_t* row_indices;
    size_t* col_ptr;
    size_t col_ptr_length;
} csc_matrix;

#endif  // CSC_MATRIX_H