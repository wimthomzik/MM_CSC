#include "csc_matrix_mult_V2.h"

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

void matr_mult_csc_V2(const void *a, const void *b, void *result) {
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

    matrixC->col_ptr = (size_t *)malloc((matrixC->cols + 1) * sizeof(size_t));
    matrixC->values = (float *)malloc((worstCaseSize) * sizeof(float));
    matrixC->row_indices = (size_t *)malloc((worstCaseSize) * sizeof(size_t));

    if (matrixC->col_ptr == NULL || matrixC->values == NULL ||
        matrixC->row_indices == NULL) {
        fprintf(stderr,
                "Failed to allocate memory for result matrix attributes\n");
        exit(EXIT_FAILURE);
    }
    // Initialize column pointers with 0
    memset(matrixC->col_ptr, 0, (matrixC->cols + 1) * sizeof(size_t));

    // Initialize values and row pointers with 0 (worst case length)
    memset(matrixC->values, 0.0, (worstCaseSize) * sizeof(float));
    memset(matrixC->row_indices, 0, (worstCaseSize) * sizeof(size_t));

    /*
    Für jede Spalte i in B:
        Initialisiere Array um errechnete Werte der Reihen zu speichern
        Für jeden Wert k in i:
            Für jeden Wert h in Spalte j von A (Spaltenindex = Zeilenindex von k): v = k*h Falls in i schon Wert für Reihe(h) berechnet: Addiere v Sonst:
                    Speichere v in C bei Reihe(h) von Spalte i
            Setze Spaltenpointer von i auf aktuelle Anzahl Elemente
    Setze nnz von C auf Anzahl Elemente
    */
    size_t *rowBuffer;
    rowBuffer = malloc(sizeof(size_t) * matrixC->rows);

    size_t valIndexC = 0;
    // Iterate through columns of matrix B
    for (size_t colIndex = 1; colIndex <= matrixC->cols; colIndex++) {
        size_t prevColPtr = matrixB->col_ptr[colIndex - 1];
        size_t currentColPtr = matrixB->col_ptr[colIndex];

        memset(rowBuffer, 0, matrixC->rows * sizeof(size_t));

        // Iterate through values in current column
        for (size_t valIndex = prevColPtr; valIndex < currentColPtr;
             valIndex++) {
            float valB = matrixB->values[valIndex];
            /*printf("%d %d:\n",valIndex,colIndex-1);*/
            // Iterate through corresponding values in A
            //  For every value in the column, we check if there are
            //  values in the column of A, which corresponds to the row index
            //  of our value in B. This is the only column said value will
            //  interact with in the process of the multiplication. If there is
            //  such a non-zero value, we multiply them and add the result to C
            //  at the point [rowIndex of A value;column Index of B value]

            // Get pointer to column of A
            size_t colPtrA = matrixB->row_indices[valIndex];

            size_t currentColPtrA = matrixA->col_ptr[colPtrA];
            size_t nextColPtrA = matrixA->col_ptr[colPtrA + 1];

            size_t numValues = nextColPtrA - currentColPtrA;
            float *resultBuffer = calloc(numValues, sizeof(float));

            size_t resultIndex = currentColPtrA;

            // Align compute index
            for (; resultIndex & 0xF && resultIndex < nextColPtrA;
                 resultIndex++) {
                resultBuffer[resultIndex - currentColPtrA] =
                    matrixA->values[resultIndex] * valB;
            }

            // Precompute result values using SIMD
            while (resultIndex + 4 <= nextColPtrA && !(resultIndex & 0xF)) {
                __m128 aVals = _mm_load_ps(matrixA->values + resultIndex);
                __m128 bVals = _mm_load1_ps(&valB);
                __m128 result = _mm_mul_ps(aVals, bVals);

                _mm_storeu_ps(resultBuffer + resultIndex - currentColPtrA,
                              result);

                resultIndex += 4;
            }

            // Allocate rest
            int remaining = nextColPtrA - resultIndex;
            switch (remaining) {
                case 1:
                    resultBuffer[resultIndex - currentColPtrA] =
                        matrixA->values[resultIndex] * valB;
                    break;
                case 2:
                    resultBuffer[resultIndex - currentColPtrA] =
                        matrixA->values[resultIndex] * valB;
                    resultBuffer[resultIndex + 1 - currentColPtrA] =
                        matrixA->values[resultIndex + 1] * valB;
                    break;
                case 3:
                    resultBuffer[resultIndex - currentColPtrA] =
                        matrixA->values[resultIndex];
                    resultBuffer[resultIndex + 1 - currentColPtrA] =
                        matrixA->values[resultIndex + 1] * valB;
                    resultBuffer[resultIndex + 2 - currentColPtrA] =
                        matrixA->values[resultIndex + 2] * valB;
                    break;
                default:
                    break;
            }

            /*printf("%d:\n",colPtrA);*/
            // Iterate through column
            for (size_t valPtrA = currentColPtrA; valPtrA < nextColPtrA;
                 valPtrA++) {
                size_t numVals = valIndex - prevColPtr;

                // Get value from A and multiply
                float res = resultBuffer[valPtrA - currentColPtrA];

                // Save at row no. rowIndexA and column no. colIndex (of B)
                size_t rowIndexA = matrixA->row_indices[valPtrA];
                /*printf("%f %f\n",valB,valA);*/

                // Check if column position already has a value
                size_t bufferVal = rowBuffer[rowIndexA];
                if (bufferVal != 0) {
                    matrixC->values[bufferVal - 1] += res;
                    continue;
                }

                // Else, append to matrix
                matrixC->values[valIndexC] += res;
                matrixC->row_indices[valIndexC] = rowIndexA;

                // Move backwards in list to appropriate position
                // if rowIndex is larger than previous element (so rows are
                // sorted)
                //  elIndex -> save of valIndexC, propagates backwards with
                //  value numVals -> number of values already added in this
                //  column, so we don't go back to a previous column's values
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
                rowBuffer[rowIndexA] = valIndexC;
            }

            free(resultBuffer);
        }
        matrixC->col_ptr[colIndex] = valIndexC;
    }
    matrixC->nnz = valIndexC;

    free(rowBuffer);
}