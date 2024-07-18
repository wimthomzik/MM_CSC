#include "csc_matrix_mult_V0.h"
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

void matr_mult_csc(const void *a, const void *b, void *result) {
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

    */

    size_t valIndexC = 0;
    //Iterate through columns of matrix B
    for(size_t colIndex = 1;colIndex <= matrixC->cols;colIndex++) {
        //Iterate through values in current column
        for(size_t valIndex = matrixB->col_ptr[colIndex-1];valIndex < matrixB->col_ptr[colIndex];valIndex++) {
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
            /*printf("%d:\n",colPtrA);*/
            //Iterate through column
            for(size_t valPtrA = matrixA->col_ptr[colPtrA]; valPtrA < matrixA->col_ptr[colPtrA+1];valPtrA++) {
                size_t numVals = valIndex - matrixB->col_ptr[colIndex-1];
                //Get value from A and multiply
                float valA = matrixA->values[valPtrA];
                float res = valA * valB;

                //Save at row no. rowIndexA and column no. colIndex (of B)
                size_t rowIndexA = matrixA->row_indices[valPtrA]; //TODO: Properly load A row_indices
                /*printf("%f %f\n",valB,valA);*/

                //Check if column position already has a value
                if(numVals != 0) {
                    int exists = 0;
                    size_t prevIndex = valIndexC;
                    //Go backwards through values in column
                    for(size_t i = valIndexC-1;i >= matrixC->col_ptr[colIndex-1];i--) {
                        if(prevIndex < i) break;
                        if(matrixC->row_indices[i] == rowIndexA) {
                            matrixC->values[i] += res;
                            exists = 1;
                            break;
                        }
                        prevIndex = i;
                    }

                    if(exists) continue;
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
            }
        }
        matrixC->col_ptr[colIndex] = valIndexC;
    }
    matrixC->nnz = valIndexC;
}