#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "csc_matrix_reader.h"


// NNZ ARE THE NON-ZERO ELEMENTS IN THE MATRIX,
// SO WE NEED TO READ THEM FIRST,
// BECAUSE THEY ARE USED TO ALLOCATE MEMORY FOR THE OTHER ARRAYS


int readCSCMatrix(const char* filename, csc_matrix* matrix) {
    // Open file
    FILE* file = fopen(filename, "r");

    // Check if file was opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    // Read matrix dimensions
    if (fscanf(file, "%d,%d", &matrix->rows, &matrix->cols) != 2) {
        fprintf(stderr, "Error reading matrix dimensions from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read values & determine nnz
    size_t capacity = 10;
    size_t nnz = 0;
    // Allocate memory for values
    matrix->values = (float*) malloc(capacity * sizeof(float));
    if (matrix->values == NULL) {
        fprintf(stderr, "Error allocating memory for values array\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read value by value


    // Allocate memory for row_indices
    matrix->row_indices = (int*) malloc(matrix->nnz * sizeof(int));
    if (matrix->row_indices == NULL) {
        fprintf(stderr, "Error allocating memory for row indices array\n");
        free(matrix->values);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read row indices


    // Allocate memory for col_ptr
    matrix->col_ptr = (int*) malloc((matrix->cols + 1) * sizeof(int));
    if (matrix->col_ptr == NULL) {
        fprintf(stderr, "Error allocating memory for col_ptr array\n");
        free(matrix->values);
        free(matrix->row_indices);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read column pointers


    // Close file
    fclose(file);
    return EXIT_SUCCESS;
}
