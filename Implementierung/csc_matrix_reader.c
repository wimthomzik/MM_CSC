#include "csc_matrix_reader.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readLine(FILE* file, float** vec, size_t* size_ptr, bool isFloat) {
    // Constants
    const size_t maxLineLength = 100;
    const size_t maxNumberLength = 10;

    // Read Line
    char lineBuffer[maxLineLength];
    char* line = fgets(lineBuffer, maxLineLength, file);
    if (line == NULL) {
        printf("Error reading values line from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Count Number of elements -> At least 1, then 1 per comma
    size_t elementCount = 1;
    for (size_t i = 0; line[i] != 0 && line[i] != '\n'; i++) {
        if (line[i] == ',') {
            elementCount++;
        }
    }

    // Address to the start of the Element Buffer
    float* elementBuffer = malloc(sizeof(float) * elementCount);
    // Address of the current position in the element buffer
    size_t elementIndex = 0;

    // Position of the start of the next element in the line buffer
    size_t elementStart = 0;

    // Necessary for null terminated strings (string parsing)
    char numberBuffer[maxNumberLength];

    for (size_t i = 0; line[i] != 0; i++) {
        // Current char
        char c = line[i];
        if (c == ',' || c == '\n') {
            // length of the current number string slice slice
            size_t len = i - elementStart;
            // copy the string slice into the numberBuffer
            strncpy(numberBuffer, &line[elementStart], len);
            // null terminate the numberBuffer for string things
            numberBuffer[len] = 0;

            // convert to number
            char* end;
            if (isFloat) {
                elementBuffer[elementIndex++] =
                    strtof((char*)&numberBuffer, &end);
            } else {
                elementBuffer[elementIndex++] =
                    strtol((char*)&numberBuffer, &end, 10);
            }

            // check for error
            if (*end != 0) {
                free(elementBuffer);
                fprintf(stderr, "Error reading matrix col ptr values\n");
                return EXIT_FAILURE;
            }

            // the new elementStart is the Position *after* the comma
            elementStart = i + 1;
        }
    }

    *vec = elementBuffer;
    *size_ptr = elementCount;

    return EXIT_SUCCESS;
}

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

    int res = readLine(file, &matrix->values, &matrix->nnz, true);
    if (res < 0) {
        fprintf(stderr, "Error reading matrix data values\n");
        fclose(file);
        return res;
    }

    res = readLine(file, (float**)&matrix->row_indices, NULL, false);
    if (res < 0) {
        free(matrix->values);
        fprintf(stderr, "Error reading matrix row index values\n");
        fclose(file);
        return res;
    }

    res = readLine(file, (float**)&matrix->col_ptr, &matrix->col_ptr_length,
                   false);
    if (res < 0) {
        free(matrix->values);
        free(matrix->row_indices);
        fprintf(stderr, "Error reading matrix col ptr values\n");
        fclose(file);
        return res;
    }

    fclose(file);
    return EXIT_SUCCESS;
}