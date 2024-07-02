#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

typedef struct {
    int rows;
    int cols;
    float* values;
    int* row_indices;
    int* col_ptr;
} csc_matrix;

#endif // CSC_MATRIX_H