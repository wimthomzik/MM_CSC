#include "csc_matrix_mult_V2.h"
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


void matr_mult_csc_V2(const void *a, const void *b, void *result) {
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

    int useBuffer = matrixC->rows <= 1000000;
    size_t* rowBuffer;
    if(useBuffer) {
        rowBuffer = malloc(sizeof(size_t)*matrixC->rows);
    }

    size_t valIndexC = 0;
    //Iterate through columns of matrix B
    for(size_t colIndex = 1;colIndex <= matrixC->cols;colIndex++) {
        size_t prevColPtr = matrixB->col_ptr[colIndex-1];
        size_t currentColPtr = matrixB->col_ptr[colIndex];


        memset(rowBuffer, 0, matrixC->rows * sizeof(size_t));

        //Iterate through values in current column
        for(size_t valIndex = prevColPtr;valIndex < currentColPtr;valIndex++) {
            float valB = matrixB->values[valIndex];
            /*printf("%d %d:\n",valIndex,colIndex-1);*/
            //Iterate through corresponding values in A
            // For every value in the column, we check if there are
            // values in the column of A, which corresponds to the row index
            // of our value in B. This is the only column said value will interact with in
            // the process of the multiplication. If there is such a non-zero value, we multiply them
            // and add the result to C at the point [rowIndex of A value;column Index of B value]

            //Get pointer to column of A
            size_t colPtrA = matrixB->row_indices[valIndex];

            size_t currentColPtrA = matrixA->col_ptr[colPtrA];
            size_t nextColPtrA = matrixA->col_ptr[colPtrA+1];

            size_t numValues = nextColPtrA-currentColPtrA;
            float* resultBuffer = calloc(numValues,sizeof(float));

            size_t resultIndex = 0;
            //Precompute result values using SIMD
            while(currentColPtrA+(resultIndex+1)*4 < nextColPtrA) {
                __m128 aVals = _mm_load_ps1((matrixA->values + resultIndex*4));
                __m128 bVals = _mm_load1_ps(&valB);
                __m128 result = _mm_mul_ps(aVals,bVals);

                _mm_storeu_ps(resultBuffer+resultIndex,result);

                resultIndex++;
            }

            
            int remaining = nextColPtrA-(resultIndex*4);
            switch(remaining) {
                case 1:
                    resultBuffer[resultIndex] = matrixA->values[resultIndex*4]*valB;
                    break;
                case 2:
                    resultBuffer[resultIndex] = matrixA->values[resultIndex*4]*valB;
                    resultBuffer[resultIndex+1] = matrixA->values[(resultIndex+1)*4]*valB;
                    break;
                case 3:
                    resultBuffer[resultIndex] = matrixA->values[resultIndex*4];
                    resultBuffer[resultIndex+1] = matrixA->values[(resultIndex+1)*4]*valB;
                    resultBuffer[resultIndex+2] = matrixA->values[(resultIndex+2)*4]*valB;
                    break;
                default:
                    break;
            }

            /*printf("%d:\n",colPtrA);*/
            //Iterate through column
            for(size_t valPtrA = currentColPtrA; valPtrA < nextColPtrA;valPtrA++) {
                size_t numVals = valIndex - prevColPtr;

                //Get value from A and multiply
                float res = resultBuffer[valPtrA-currentColPtrA];

                //Save at row no. rowIndexA and column no. colIndex (of B)
                size_t rowIndexA = matrixA->row_indices[valPtrA];
                /*printf("%f %f\n",valB,valA);*/

                //Check if column position already has a value
                size_t bufferVal = rowBuffer[rowIndexA];
                if(bufferVal != 0) {
                    matrixC->values[bufferVal-1] += res;
                    continue;
                }

                //Else, append to matrix
                matrixC->values[valIndexC] += res;
                matrixC->row_indices[valIndexC] = rowIndexA;

                //Move backwards in list to appropriate position 
                //if rowIndex is larger than previous element
                size_t elIndex = valIndexC;
                while(numVals > 0 && matrixC->row_indices[elIndex-1] > rowIndexA) {
                    matrixC->values[elIndex] = matrixC->values[elIndex-1];
                    matrixC->values[elIndex-1] = res;

                    matrixC->row_indices[elIndex] = matrixC->row_indices[elIndex-1];
                    matrixC->row_indices[elIndex-1] = rowIndexA;

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

void csc_to_csr(const void* m) {
    const csc_matrix *mat = (const csc_matrix *) m;
    
    size_t rows = mat->rows;
    size_t nnz = mat->nnz;

    // Allocate memory for CSR format arrays
    float *csr_values = (float *)malloc(nnz * sizeof(float));
    size_t *csr_col_indices = (size_t *)malloc(nnz * sizeof(size_t));
    size_t *csr_row_pointers = (size_t *)malloc((rows + 1) * sizeof(size_t));

    // Step 2: Count the number of entries in each row
    size_t *row_count = (size_t *)calloc(rows, sizeof(size_t));
    for (size_t i = 0; i < nnz; ++i) {
        row_count[mat->row_indices[i]]++;
    }

    // Step 3: Compute row pointers
    csr_row_pointers[0] = 0;
    for (size_t i = 1; i <= rows; ++i) {
        csr_row_pointers[i] = csr_row_pointers[i - 1] + row_count[i - 1];
    }

    // Temporary array to hold the current position in each row
    size_t *current_pos = (size_t *)malloc(rows * sizeof(size_t));
    for (size_t i = 0; i < rows; ++i) {
        current_pos[i] = csr_row_pointers[i];
    }

    // Step 4: Fill CSR values and column indices
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

    // Free temporary arrays
    free(row_count);
    free(current_pos);

    for (size_t i = 0; i < mat->nnz; i++) { 
        mat->values[i] = csr_values[i]; 
        mat->row_indices[i] = csr_col_indices[i]; 
    } 

    for (size_t i = 0; i <= mat->cols; i++) { 
        mat->col_ptr[i] = csr_row_pointers[i]; 
    } 
}