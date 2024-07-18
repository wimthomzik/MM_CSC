#include "csc_matrix_mult_V3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*Include allowed intrinsics*/
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <tmmintrin.h>
#include <smmintrin.h>
#include <nmmintrin.h>

void matr_mult_csc_V3(const void *a, const void *b, void *result) {
    const csc_matrix *matrixA = (const csc_matrix *) a;
    const csc_matrix *matrixB = (const csc_matrix *) b;
    csc_matrix *matrixC = (csc_matrix *) result;

    // Check if matrix dimensions are compatible for multiplication
    if (matrixA->cols != matrixB->rows) {
        fprintf(stderr, "Matrix dimensions do not match for multiplication: %zd (A.cols) != %zd (B.rows)\n", matrixA->cols, matrixB->rows);
        exit(EXIT_FAILURE);
    }
    // Initialize result matrix
    matrixC->rows = matrixA->rows;
    matrixC->cols = matrixB->cols;
    matrixC->nnz = 0;
    matrixC->values = NULL;
    matrixC->row_indices = NULL;

    size_t worstCaseSize = (matrixA->nnz) * (matrixB->nnz);

    matrixC->col_ptr = (size_t*) malloc((matrixC->cols + 1) * sizeof(size_t));
    matrixC->values = (float*) malloc((worstCaseSize) * sizeof(float));
    matrixC->row_indices = (size_t*) malloc((worstCaseSize) * sizeof(size_t));

    if (matrixC->col_ptr == NULL ||
        matrixC->values == NULL ||
        matrixC->row_indices == NULL) {
        fprintf(stderr, "Failed to allocate memory for result matrix attributes\n");
        exit(EXIT_FAILURE);
    }
    // Initialize column pointers with 0
    memset(matrixC->col_ptr, 0, (matrixC->cols + 1) * sizeof(size_t));

    //Initialize values and row pointers with 0 (worst case length)
    memset(matrixC->values, 0.0, (worstCaseSize) * sizeof(float));
    memset(matrixC->row_indices, 0, (worstCaseSize) * sizeof(size_t));

    /*
    Test case 3:
    float valuesA[16] = {2,12,4,5,3,7,2,3,6,3,1,7,9,2,6,10};
    size_t row_indicesA[16] = {0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3};
    size_t col_ptrA[5] = {0,4,8,12,16};

    float valuesB[26] = {6,20,12,1,8,6,3,2,7,3,9,1,6,9,8,7,3,15,11,9,8,5,3,13,1,2};
    size_t row_indicesB[26] = {0,1,2,3,0,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,1,2,3,0,2,3};
    size_t col_ptrB[8] = {0,4,7,11,15,19,23,26};
Í
    */

    //csc_to_csr(matrixA);


    size_t *rowBuffer = (size_t *)calloc(matrixA->rows, sizeof(size_t));

    size_t valIndexC = 0;
    //Iterate through columns of matrix B
    for(size_t colIndex = 0;colIndex < matrixC->cols;colIndex++) {
        size_t currentColPtrB = matrixB->col_ptr[colIndex];
        size_t nextColPtrB = matrixB->col_ptr[colIndex+1];

        for(size_t valIndexB = currentColPtrB;valIndexB < nextColPtrB; valIndexB++) {
            size_t rowB = matrixB->row_indices[valIndexB];
            float valB = matrixB->values[valIndexB];

            size_t currentColPtrA = matrixA->col_ptr[rowB];
            size_t nextColPtrA = matrixA->col_ptr[rowB+1];

            size_t valIndexA = currentColPtrA;
            for(;valIndexA & 0xF;valIndexA++) {
                size_t rowA = matrixA->row_indices[valIndexA];
                float valA = matrixA->values[valIndexA];

                if (rowBuffer[rowA] < matrixC->col_ptr[colIndex]+1) {
                    rowBuffer[rowA] = valIndexC+1;
                    matrixC->row_indices[valIndexC] = rowA;
                    valIndexC++;
                }
                matrixC->values[rowBuffer[rowA]-1] += valA*valB;
            }

            for(;valIndexA < nextColPtrA-4; valIndexA+=4) {
                size_t rowA = matrixA->row_indices[valIndexA];
                size_t rowA1 = matrixA->row_indices[valIndexA+1];
                size_t rowA2 = matrixA->row_indices[valIndexA+2];
                size_t rowA3 = matrixA->row_indices[valIndexA+3];
                

                if (rowBuffer[rowA] < matrixC->col_ptr[colIndex]+1) {
                    rowBuffer[rowA] = valIndexC+1;
                    matrixC->row_indices[valIndexC] = rowA;
                    valIndexC++;
                }
                if (rowBuffer[rowA1] < matrixC->col_ptr[colIndex]+1) {
                    rowBuffer[rowA1] = valIndexC+1;
                    matrixC->row_indices[valIndexC] = rowA1;
                    valIndexC++;
                }
                if (rowBuffer[rowA2] < matrixC->col_ptr[colIndex]+1) {
                    rowBuffer[rowA2] = valIndexC+1;
                    matrixC->row_indices[valIndexC] = rowA2;
                    valIndexC++;
                }
                if (rowBuffer[rowA3] < matrixC->col_ptr[colIndex]+1) {
                    rowBuffer[rowA3] = valIndexC+1;
                    matrixC->row_indices[valIndexC] = rowA3;
                    valIndexC++;
                }


                __m128 aVals = _mm_load_ps(matrixA->values + valIndexA);
                __m128 bVals = _mm_load1_ps(&valB);

                __m128 result = _mm_mul_ps(aVals,bVals);

                float resultArr[4];
                _mm_storeu_ps(resultArr,result);


                matrixC->values[rowBuffer[rowA]-1] += resultArr[0];
                matrixC->values[rowBuffer[rowA1]-1] += resultArr[1];
                matrixC->values[rowBuffer[rowA2]-1] += resultArr[2];
                matrixC->values[rowBuffer[rowA3]-1] += resultArr[3];
            }

            for(;valIndexA < nextColPtrA; valIndexA++) {
                size_t rowA = matrixA->row_indices[valIndexA];
                float valA = matrixA->values[valIndexA];

                if (rowBuffer[rowA] < matrixC->col_ptr[colIndex]+1) {
                    rowBuffer[rowA] = valIndexC+1;
                    matrixC->row_indices[valIndexC] = rowA;
                    valIndexC++;
                }
                matrixC->values[rowBuffer[rowA]-1] += valA*valB;
            }
        }

        matrixC->col_ptr[colIndex+1] = valIndexC;
    }
    matrixC->nnz = valIndexC;

    free(rowBuffer);
}

void csc_to_csr_V3(const void* m) {
    const csc_matrix *mat = (const csc_matrix *) m;
    
    size_t rows = mat->rows;
    size_t nnz = mat->nnz;

    //Allocate memory for CSR format arrays
    float *csr_values = (float *)malloc(nnz * sizeof(float));
    size_t *csr_col_indices = (size_t *)malloc(nnz * sizeof(size_t));
    size_t *csr_row_pointers = (size_t *)malloc((rows + 1) * sizeof(size_t));

    //Count number of entries in each row
    size_t *row_count = (size_t *)calloc(rows, sizeof(size_t));
    for (size_t i = 0; i < nnz; ++i) {
        row_count[mat->row_indices[i]]++;
    }

    //Calculate row pointers
    csr_row_pointers[0] = 0;
    for (size_t i = 1; i <= rows; ++i) {
        csr_row_pointers[i] = csr_row_pointers[i - 1] + row_count[i - 1];
    }

    size_t *current_pos = (size_t *)malloc(rows * sizeof(size_t));
    for (size_t i = 0; i < rows; ++i) {
        current_pos[i] = csr_row_pointers[i];
    }

    //Fill CSR values and column indices
    for (size_t col = 0; col < mat->cols; ++col) {
        size_t col_start = mat->col_ptr[col];
        size_t col_end = mat->col_ptr[col + 1];
        for (size_t idx = col_start; idx < col_end; ++idx) {
            size_t row = mat->row_indices[idx];
            size_t dest = current_pos[row];
            csr_values[dest] = mat->values[idx];
            csr_col_indices[dest] = col;
            current_pos[row]++;
        }
    }

    free(row_count);
    free(current_pos);

    //Transfer values
    for (size_t i = 0; i < mat->nnz; i++) { 
        mat->values[i] = csr_values[i]; 
        mat->row_indices[i] = csr_col_indices[i]; 
    } 

    for (size_t i = 0; i <= mat->cols; i++) { 
        mat->col_ptr[i] = csr_row_pointers[i]; 
    } 
}