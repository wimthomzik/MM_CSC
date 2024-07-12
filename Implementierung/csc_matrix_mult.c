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
    matrixC->nnz = (matrixA->nnz) * (matrixB->nnz);
    matrixC->values = NULL;
    matrixC->row_indices = NULL;

    size_t worstCaseSize = 5*5; // TODO: Change to nnz when working
    matrixC->col_ptr = (int*) malloc((matrixC->cols + 1) * sizeof(int));
    matrixC->values = (float*) malloc((worstCaseSize) * sizeof(float));
    matrixC->row_indices = (int*) malloc((worstCaseSize) * sizeof(int));

    if (matrixC->col_ptr == NULL ||
        matrixC->values == NULL ||
        matrixC->row_indices == NULL) {
        fprintf(stderr, "Failed to allocate memory for result matrix attributes\n");
        exit(EXIT_FAILURE);
    }
    // Initialize column pointers with 0
    memset(matrixC->col_ptr, 0, (matrixC->cols + 1) * sizeof(int));

    //Initialize values and row pointers with 0 (worst case length)
    memset(matrixC->values, 0.0, (worstCaseSize) * sizeof(float));
    memset(matrixC->row_indices, 0, (worstCaseSize) * sizeof(int));

    //Test values
    float valuesA[6] = {25.000000,0.500000,2.500000,36.000000,6.000000,9.000000};
    int row_indicesA[6] = {0,1,2,1,1,3};
    int col_ptrA[5] = {0,3,4,5,6};

    float valuesB[6] = {5,0.5,6,1,3};
    int row_indicesB[6] = {0,2,1,1,3};
    int col_ptrB[5] = {0,2,3,4,5};

    memcpy(matrixA->values, valuesA, sizeof(valuesA));
    memcpy(matrixB->values, valuesB, sizeof(valuesB));
    //memcpy(matrixC->values, values, sizeof(values));

    memcpy(matrixA->row_indices, row_indicesA, sizeof(row_indicesA));
    memcpy(matrixB->row_indices, row_indicesB, sizeof(row_indicesB));
    //memcpy(matrixC->row_indices, row_indices, sizeof(row_indices));

    memcpy(matrixA->col_ptr, col_ptrA, sizeof(col_ptrA));
    memcpy(matrixB->col_ptr, col_ptrB, sizeof(col_ptrB));
    //memcpy(matrixC->col_ptr, col_ptr, sizeof(col_ptr));


    
    int valIndexC = 0;
    //Iterate through columns of matrix B
    for(int colIndex = 1;colIndex <= matrixB->cols;colIndex++) {
        //Iterate through values in current column
        for(int valIndex = matrixB->col_ptr[colIndex-1];valIndex < matrixB->col_ptr[colIndex];valIndex++) {
            int numVals = valIndex - matrixB->col_ptr[colIndex-1];
            float valB = matrixB->values[valIndex];
            printf("%d %d:\n",valIndex,colIndex);
            //Iterate through corresponding values in A
            // For every value in the column, we check if there are
            // values in the column of A, which corresponds to the row index
            // of our value in B. If there is such a non-zero value, we multiply them
            // and add the result to C at the point [rowIndex of A value;column Index of B value]

            //Get pointer to column of A
            int colPtrA = matrixB->row_indices[valIndex];
            //Iterate through column
            for(int valPtrA = matrixA->col_ptr[colPtrA]; valPtrA < matrixA->col_ptr[colPtrA+1];valPtrA++) {
                //Get value from A and multiply
                float valA = matrixA->values[valPtrA];
                float res = valA * valB;

                //Save at row no. rowIndexA and column no. colIndex (of B)
                int rowIndexA = row_indicesA[valPtrA]; //TODO: Properly load A row_indices
                printf("%f %f\n",valB,valA);

                //Check if column position already has a value
                if(numVals != 0) {
                    int exists = 0;
                    for(int i = valIndexC;i >= matrixC->col_ptr[colIndex-1];i--) {
                        if(matrixC->row_indices[i] == rowIndexA) {
                            matrixC->values[i] += res;
                            exists = 1;
                            break;
                        }
                    }

                    if(exists) continue;
                }

                //Else, append to matrix
                matrixC->values[valIndexC] += res;
                matrixC->row_indices[valIndexC] = rowIndexA;

                //Move backwards in list to appropriate position 
                //if rowIndex is larger than previous element
                int elIndex = valIndexC;
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
            matrixC->col_ptr[colIndex] = valIndexC;
        }
    }
    matrixC->nnz = valIndexC;
}
