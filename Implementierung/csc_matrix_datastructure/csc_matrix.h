#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

typedef struct {
    int rows;
    int cols;
    int nnz;         // Number of non-zero values
    float* values;
    int* row_indices;
    int* col_ptr;
} csc_matrix;

#endif // CSC_MATRIX_H