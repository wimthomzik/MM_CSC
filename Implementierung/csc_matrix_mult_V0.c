#include "csc_matrix_mult_V0.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Include allowed intrinsics*/
#include <xmmintrin.h>

void matr_mult_csc(const void *a, const void *b, void *result) {
    const csc_matrix *matrixA = (const csc_matrix *)a;
    const csc_matrix *matrixB = (const csc_matrix *)b;
    csc_matrix *matrixC = (csc_matrix *)result;

    // Check if matrix dimensions are compatible for multiplication
    if (matrixA->cols != matrixB->rows) {
        fprintf(stderr,
                "Matrix dimensions do not match for multiplication: %zd "
                "(A.cols) != %zd (B.rows)\n",
                matrixA->cols, matrixB->rows);
        exit(EXIT_FAILURE);
    }
    // Initialize result matrix
    matrixC->rows = matrixA->rows;
    matrixC->cols = matrixB->cols;
    matrixC->nnz = 0;
    matrixC->col_ptr = NULL;
    matrixC->values = NULL;
    matrixC->row_indices = NULL;

    size_t worstCaseSize = (matrixA->nnz) * (matrixB->nnz);

    matrixC->col_ptr = (size_t *)calloc((matrixC->cols + 1), sizeof(size_t));
    matrixC->values = (float *)calloc((worstCaseSize), sizeof(float));
    matrixC->row_indices = (size_t *)calloc((worstCaseSize), sizeof(size_t));

    if (matrixC->col_ptr == NULL || matrixC->values == NULL ||
        matrixC->row_indices == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for result matrix attributes\n");
        exit(EXIT_FAILURE);
    }

    /*
    Pseudocode:

    Für jede Spalte i in B:
        Für jeden Wert k in i:
            Für jeden Wert h in Spalte j von A (Spaltenindex = Zeilenindex von k): v = k*h Falls in i schon Wert für Reihe(h) berechnet: Addiere v Sonst:
                    Speichere v in C bei Reihe(h) von Spalte i
            Setze Spaltenpointer von i auf aktuelle Anzahl Elemente
    Setze nnz von C auf Anzahl Elemente
    */
    size_t valIndexC = 0;
    // Iterate through columns of matrix B
    for (size_t colIndex = 1; colIndex <= matrixC->cols; colIndex++) {
        
        // Iterate through values in current column
        for (size_t valIndex = matrixB->col_ptr[colIndex - 1];
             valIndex < matrixB->col_ptr[colIndex]; valIndex++) {
            float valB = matrixB->values[valIndex];

            // Get pointer to column of A
            size_t colPtrA = matrixB->row_indices[valIndex];
            
            // Iterate through column
            for (size_t valPtrA = matrixA->col_ptr[colPtrA];
                 valPtrA < matrixA->col_ptr[colPtrA + 1]; valPtrA++) {
                size_t numVals = valIndex - matrixB->col_ptr[colIndex - 1];
                // Get value from A and multiply
                float valA = matrixA->values[valPtrA];
                float res = valA * valB;

                // Save at row no. rowIndexA and column no. colIndex (of B)
                size_t rowIndexA = matrixA->row_indices[valPtrA];

                // Check if column position already has a value
                if (numVals != 0) {
                    int exists = 0;
                    size_t prevIndex = valIndexC;
                    // Go backwards through values in column
                    for (size_t i = valIndexC - 1;
                         i >= matrixC->col_ptr[colIndex - 1]; i--) {
                        if (prevIndex < i) break;
                        if (matrixC->row_indices[i] == rowIndexA) {
                            matrixC->values[i] += res;
                            exists = 1;
                            break;
                        }
                        prevIndex = i;
                    }

                    if (exists) continue;
                }

                // Else, append to matrix
                matrixC->values[valIndexC] += res;
                matrixC->row_indices[valIndexC] = rowIndexA;

                // Move backwards in list to appropriate position
                size_t elIndex = valIndexC;
                while (numVals > 0 &&
                       matrixC->row_indices[elIndex - 1] > rowIndexA) {
                    matrixC->values[elIndex] = matrixC->values[elIndex - 1];
                    matrixC->values[elIndex - 1] = res;

                    matrixC->row_indices[elIndex] =
                        matrixC->row_indices[elIndex - 1];
                    matrixC->row_indices[elIndex - 1] = rowIndexA;

                    numVals--;
                    elIndex--;
                }

                valIndexC++;
            }
        }
        matrixC->col_ptr[colIndex] = valIndexC;
    }
    matrixC->nnz = valIndexC;
}