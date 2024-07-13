#include "csc_matrix_reader.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

    int res = readFloatLine(file, &matrix->values, &matrix->nnz);
    if (res < 0) {
        return res;
    }

    res = readIntLine(file, &matrix->row_indices, NULL);
    if (res < 0) {
        return res;
    }

    res = readIntLine(file, &matrix->col_ptr, &matrix->col_ptr_length);
    if (res < 0) {
        return res;
    }

    fclose(file);
    return 0;
}

int readIntLine(FILE* file, int** vec, size_t* length) {
    const size_t maxLineLength = 100;

    char line[maxLineLength];
    char* valuesLine = fgets(line, maxLineLength, file);
    if (valuesLine == NULL) {
        printf("Error reading values line from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    size_t elementCount = 1;
    for (size_t i = 0; valuesLine[i] != 0 && valuesLine[i] != '\n'; i++) {
        if (valuesLine[i] == ',') {
            elementCount++;
        }
    }

    int* elements = malloc(sizeof(int) * elementCount);
    size_t elIndex = 0;
    size_t elStart = 0;
    const int maxNumberLength = 10;
    char numberBuffer[maxNumberLength];

    for (size_t i = 0; valuesLine[i] != 0; i++) {
        char c = valuesLine[i];
        if (c == ',' || c == '\n') {
            size_t len = i - elStart;
            strncpy(numberBuffer, &valuesLine[elStart], len);
            elStart = i + 1;
            numberBuffer[len] = 0;
            elements[elIndex++] = atoi(numberBuffer);
        }
    }

    *vec = elements;
    *length = elementCount;

    return EXIT_SUCCESS;
}
int readFloatLine(FILE* file, float** vec, size_t* length) {
    const size_t maxLineLength = 100;

    char line[maxLineLength];
    char* valuesLine = fgets(line, maxLineLength, file);
    if (valuesLine == NULL) {
        printf("Error reading values line from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    size_t elementCount = 1;
    for (size_t i = 0; valuesLine[i] != 0 && valuesLine[i] != '\n'; i++) {
        if (valuesLine[i] == ',') {
            elementCount++;
        }
    }

    float* elements = malloc(sizeof(float) * elementCount);
    size_t elIndex = 0;
    size_t elStart = 0;
    const int maxNumberLength = 10;
    char numberBuffer[maxNumberLength];

    for (size_t i = 0; valuesLine[i] != 0; i++) {
        char c = valuesLine[i];
        if (c == ',' || c == '\n') {
            size_t len = i - elStart;
            strncpy(numberBuffer, &valuesLine[elStart], len);
            elStart = i + 1;
            numberBuffer[len] = 0;
            elements[elIndex++] = atof(numberBuffer);
        }
    }

    *vec = elements;
    if (length != NULL) {
        *length = elementCount;
    }

    return EXIT_SUCCESS;
}