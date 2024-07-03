#include "csc_matrix_mult.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void matr_mult_csc(const void *a, const void *b, void *result) {
    const csc_matrix *matrixA = (const csc_matrix *) a;
    const csc_matrix *matrixB = (const csc_matrix *) b;
    csc_matrix *matrixC = (csc_matrix *) result;

    // Check if matrix dimensions are compatible for multiplication
    if (matrixA->cols != matrixB->rows) {
        fprintf(stderr, "Matrix dimensions do not match for multiplication: %d (A.cols) != %d (B.rows)\n", matrixA->cols, matrixB->rows);
        exit(EXIT_FAILURE);
    }

    // Initialize result matrix
    matrixC->rows = matrixA->rows;
    matrixC->cols = matrixB->cols;
    matrixC->nnz = 0;
    matrixC->values = NULL;
    matrixC->row_indices = NULL;
    matrixC->col_ptr = (int*) malloc((matrixC->cols + 1) * sizeof(int));
    if (matrixC->col_ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory for result matrix column pointers\n");
        exit(EXIT_FAILURE);
    }
    // Initialize column pointers with 0
    memset(matrixC->col_ptr, 0, (matrixC->cols + 1) * sizeof(int));

}
