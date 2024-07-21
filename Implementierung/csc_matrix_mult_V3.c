#include "csc_matrix_mult_V3.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Include allowed intrinsics*/
#include <emmintrin.h>
#include <nmmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#include <tmmintrin.h>
#include <xmmintrin.h>

void matr_mult_csc_V3(const void *a, const void *b, void *result) {
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

    // csc_to_csr(matrixA);

    /*
    Für jede Spalte i in B:
        Initialisiere Array um errechnete Werte der Reihen zu speichern
        Für jeden Wert k in i:
            Für jeden Wert h in Spalte j von A (Spaltenindex = Zeilenindex von k): v = k*h Falls in i schon Wert für Reihe(h) berechnet: Addiere v Sonst:
                    Speichere v in C bei Reihe(h) von Spalte i
            Setze Spaltenpointer von i auf aktuelle Anzahl Elemente
    Setze nnz von C auf Anzahl Elemente
    */

    size_t *rowBuffer = (size_t *)calloc(matrixA->rows,sizeof(size_t));
    size_t valIndexC = 0;
    // Iterate through columns of matrix B
    for (size_t colIndex = 0; colIndex < matrixC->cols; colIndex++) {

        size_t currentColPtrB = matrixB->col_ptr[colIndex];
        size_t nextColPtrB = matrixB->col_ptr[colIndex + 1];
        for (size_t valIndexB = currentColPtrB; valIndexB < nextColPtrB;valIndexB++) {
            size_t rowB = matrixB->row_indices[valIndexB];
            float valB = matrixB->values[valIndexB];

            size_t currentColPtrA = matrixA->col_ptr[rowB];
            size_t nextColPtrA = matrixA->col_ptr[rowB + 1];

            size_t valIndexA = currentColPtrA;
            for (; valIndexA & 0xF && valIndexA < nextColPtrA; valIndexA++) {
                size_t rowA = matrixA->row_indices[valIndexA];
                float valA = matrixA->values[valIndexA];

                if (rowBuffer[rowA] < matrixC->col_ptr[colIndex] + 1) {
                    rowBuffer[rowA] = valIndexC + 1;
                    matrixC->row_indices[valIndexC] = rowA;
                    valIndexC++;
                }
                matrixC->values[rowBuffer[rowA] - 1] += valA * valB;
            }
            // Directly use computed values
            // Leads to easier structure and benefits from memory locality
            for (; valIndexA < nextColPtrA - 4 && !(valIndexA & 0xF);valIndexA += 4) {
                size_t rowA = matrixA->row_indices[valIndexA];
                size_t rowA1 = matrixA->row_indices[valIndexA + 1];
                size_t rowA2 = matrixA->row_indices[valIndexA + 2];
                size_t rowA3 = matrixA->row_indices[valIndexA + 3];

                if (rowBuffer[rowA] < matrixC->col_ptr[colIndex] + 1) {
                    rowBuffer[rowA] = valIndexC + 1;
                    matrixC->row_indices[valIndexC] = rowA;
                    valIndexC++;
                }
                if (rowBuffer[rowA1] < matrixC->col_ptr[colIndex] + 1) {
                    rowBuffer[rowA1] = valIndexC + 1;
                    matrixC->row_indices[valIndexC] = rowA1;
                    valIndexC++;
                }
                if (rowBuffer[rowA2] < matrixC->col_ptr[colIndex] + 1) {
                    rowBuffer[rowA2] = valIndexC + 1;
                    matrixC->row_indices[valIndexC] = rowA2;
                    valIndexC++;
                }
                if (rowBuffer[rowA3] < matrixC->col_ptr[colIndex] + 1) {
                    rowBuffer[rowA3] = valIndexC + 1;
                    matrixC->row_indices[valIndexC] = rowA3;
                    valIndexC++;
                }

                __m128 aVals = _mm_load_ps(matrixA->values + valIndexA);
                __m128 bVals = _mm_load1_ps(&valB);

                __m128 result = _mm_mul_ps(aVals, bVals);

                float* resultArr = malloc(4*sizeof(float));
                _mm_storeu_ps(resultArr, result);

                matrixC->values[rowBuffer[rowA] - 1] += resultArr[0];
                matrixC->values[rowBuffer[rowA1] - 1] += resultArr[1];
                matrixC->values[rowBuffer[rowA2] - 1] += resultArr[2];
                matrixC->values[rowBuffer[rowA3] - 1] += resultArr[3];
                free(resultArr);
            }

            /*
            Change from V2:
            rowBuffer now also prevents the need to move added values backwards
            through the arrays.
            */
            for (; valIndexA < nextColPtrA; valIndexA++) {
                size_t rowA = matrixA->row_indices[valIndexA];
                float valA = matrixA->values[valIndexA];

                if (rowBuffer[rowA] < matrixC->col_ptr[colIndex] + 1) {
                    rowBuffer[rowA] = valIndexC + 1;
                    matrixC->row_indices[valIndexC] = rowA;
                    valIndexC++;
                }
                matrixC->values[rowBuffer[rowA] - 1] += valA * valB;
            }
        }

        matrixC->col_ptr[colIndex + 1] = valIndexC;
    }
    matrixC->nnz = valIndexC;
    free(rowBuffer);

}