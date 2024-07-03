#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "csc_matrix_reader.h"

int readCSCMatrix(const char* filename, csc_matrix* matrix) {
    // Open file
    FILE* file = fopen(filename, "r");

    // Check if file was opened successfully
    if (file == NULL) {
        perror("Error opening file");
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
        perror("Error allocating memory for values array");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read value by value
    float value;
    while (fscanf(file, "%f", &value) == 1) {
        if (nnz >= capacity) {
            capacity *= 2;
            float* temp = (float*) realloc(matrix->values, capacity * sizeof(float));
            if (temp == NULL) {
                perror("Error reallocating memory for values array");
                free(matrix->values);
                fclose(file);
                return EXIT_FAILURE;
            }
            matrix->values = temp;
        }
        matrix->values[nnz++] = value;
    }
    matrix->nnz = nnz;

    // Allocate memory for row_indices
    matrix->row_indices = (int*) malloc(matrix->nnz * sizeof(int));
    if (matrix->row_indices == NULL) {
        perror("Error allocating memory for row indices array");
        free(matrix->values);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read row indices
    for (size_t i = 0; i < matrix->nnz; i++) {
        if (fscanf(file, "%d", &matrix->row_indices[i]) != 1) {
            fprintf(stderr, "Error reading row indices from file.\n");
            free(matrix->values);
            free(matrix->row_indices);
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    // Allocate memory for col_ptr
    matrix->col_ptr = (int*) malloc((matrix->cols + 1) * sizeof(int));
    if (matrix->col_ptr == NULL) {
        perror("Error allocating memory for col_ptr array");
        free(matrix->values);
        free(matrix->row_indices);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read column pointers
    for (size_t i = 0; i < matrix->cols + 1; i++) {
        if (fscanf(file, "%d", &matrix->col_ptr[i]) != 1) {
            fprintf(stderr, "Error reading column pointers from file.\n");
            free(matrix->values);
            free(matrix->row_indices);
            free(matrix->col_ptr);
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    // Close file
    fclose(file);
    return EXIT_SUCCESS;
}
