#include <stdio.h>
#include <stdlib.h>
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

    // Allocate memory for matrix values

    // Allocate memory for row indices

    // Allocate memory for column pointer

    // Read matrix values

    // Read row indices

    // Read column pointer

    // Close file
    fclose(file);
    return EXIT_SUCCESS;
}
