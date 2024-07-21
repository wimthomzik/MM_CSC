#include "csc_matrix_printer.h"

#include <stdio.h>

void printCSCMatrix(const csc_matrix* matrix) {
    if (matrix == NULL) {
        fprintf(stderr, "Error: No matrix found.\n");
        return;
    }

    // Print dimensions
    printf("%zu,%zu\n", matrix->rows, matrix->cols);

    // Print values
    if (matrix->nnz > 0 && matrix->values != NULL) {
        for (size_t i = 0; i < matrix->nnz; i++) {
            if (i < matrix->nnz - 1)
                printf("%.2f,", matrix->values[i]);
            else
                printf("%.2f\n", matrix->values[i]);
        }
    } else {
        printf("\n");
    }

    // Print row indices
    if (matrix->nnz > 0 && matrix->row_indices != NULL) {
        for (size_t i = 0; i < matrix->nnz; i++) {
            if (i < matrix->nnz - 1)
                printf("%zu,", matrix->row_indices[i]);
            else
                printf("%zu\n", matrix->row_indices[i]);
        }
    } else {
        printf("\n");
    }

    // Print column pointers
    if (matrix->col_ptr_length > 0 && matrix->col_ptr != NULL) {
        for (size_t i = 0; i < matrix->col_ptr_length; i++) {
            if (i < matrix->col_ptr_length - 1)
                printf("%zu,", matrix->col_ptr[i]);
            else
                printf("%zu\n", matrix->col_ptr[i]);
        }
    } else {
        printf("\n");
    }
}