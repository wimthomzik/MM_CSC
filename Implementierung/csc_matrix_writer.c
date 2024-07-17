#include "csc_matrix_writer.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int writeCSCMatrix(const char *filename, const csc_matrix *matrix) {
    if (filename == NULL || matrix == NULL) {
        fprintf(stderr, "Invalid filename or matrix pointer\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s: %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    // Write number of rows and columns
    if (fprintf(file, "%zd,%zd\n", matrix->rows, matrix->cols) < 0) {
        fprintf(stderr, "Failed to write matrix dimensions to %s\n", filename);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Write values
    for (size_t i = 0; i < matrix->nnz; i++) {
        if (fprintf(file, "%f", matrix->values[i]) < 0) {
            fprintf(stderr, "Failed to write matrix values to %s\n", filename);
            fclose(file);
            return EXIT_FAILURE;
        }

        if (i == matrix->nnz - 1) {
            break;
        }

        if (fprintf(file, ",") < 0) {
            fprintf(stderr, "Failed to write matrix values to %s\n", filename);
            fclose(file);
            return EXIT_FAILURE;
        }
    }
    if (fprintf(file, "\n") < 0) {
        fprintf(stderr, "Failed to write matrix values to %s\n", filename);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Write row indices
    for (size_t i = 0; i < matrix->nnz; i++) {
        if (fprintf(file, "%zd", matrix->row_indices[i]) < 0) {
            fprintf(stderr, "Failed to write row indices to %s\n", filename);
            fclose(file);
            return EXIT_FAILURE;
        }

        if (i == matrix->nnz - 1) {
            break;
        }
        
        if (fprintf(file, ",") < 0) {
            fprintf(stderr, "Failed to write row indices to %s\n", filename);
            fclose(file);
            return EXIT_FAILURE;
        }
    }
    if (fprintf(file, "\n") < 0) {
        fprintf(stderr, "Failed to write row indices to %s\n", filename);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Write column pointers
    for (size_t i = 0; i <= matrix->cols; i++) {
        if (fprintf(file, "%zd", matrix->col_ptr[i]) < 0) {
            fprintf(stderr, "Failed to write column pointers to %s\n", filename);
            fclose(file);
            return EXIT_FAILURE;
        }
        if (i < matrix->cols) {
            if (fprintf(file, ",") < 0) {
                fprintf(stderr, "Failed to write column pointers to %s\n", filename);
                fclose(file);
                return EXIT_FAILURE;
            }
        }
    }
    if (fprintf(file, "\n") < 0) {
        fprintf(stderr, "Failed to write column pointers to %s\n", filename);
        fclose(file);
        return EXIT_FAILURE;
    }

    fclose(file);
    return EXIT_SUCCESS;
}
