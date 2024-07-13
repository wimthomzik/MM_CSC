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

    int res = readFloatLine(file, &matrix->values);
    if (res != 0) {
        return res;
    }

    res = readIntLine(file, &matrix->row_indices);
    if (res != 0) {
        return res;
    }

    res = readIntLine(file, &matrix->col_ptr);
    if (res != 0) {
        return res;
    }

    fclose(file);
    return 0;
}

int readIntLine(FILE* file, intvec* vec) {
    const size_t maxLineLength = 100;

    char line[maxLineLength];
    char* valuesLine = fgets(line, maxLineLength, file);
    if (valuesLine == NULL) {
        printf("Error reading values line from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    size_t elementCount = 1;
    for (int i = 0; valuesLine[i] != 0 && valuesLine[i] != '\n'; i++) {
        if (valuesLine[i] == ',') {
            elementCount++;
        }
    }

    float* elements = malloc(sizeof(float) * elementCount);
    int elIndex = 0;
    int elStart = 0;
    const size_t maxNumberLength = 10;
    char numberBuffer[maxNumberLength];

    for (int i = 0; valuesLine[i] != 0; i++) {
        char c = valuesLine[i];
        if (c == ',' || c == '\n') {
            size_t len = i - elStart;
            // copy number to buffer for atoi()
            strncpy(numberBuffer, &valuesLine[elStart], len);
            elStart = i + 1;  // continue after comma
            numberBuffer[len] = 0;
            elements[elIndex++] = atoi(numberBuffer);
        }
    }

    vec->data = elements;
    vec->length = elementCount;

    return 0;
}

int readFloatLine(FILE* file, floatvec* vec) {
    const size_t maxLineLength = 100;

    char line[maxLineLength];
    char* valuesLine = fgets(line, maxLineLength, file);
    if (valuesLine == NULL) {
        printf("Error reading values line from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    size_t elementCount = 1;
    for (int i = 0; valuesLine[i] != 0 && valuesLine[i] != '\n'; i++) {
        if (valuesLine[i] == ',') {
            elementCount++;
        }
    }

    float* elements = malloc(sizeof(float) * elementCount);
    int elIndex = 0;
    int elStart = 0;
    const size_t maxNumberLength = 10;
    char numberBuffer[maxNumberLength];

    for (int i = 0; valuesLine[i] != 0; i++) {
        char c = valuesLine[i];
        if (c == ',' || c == '\n') {
            size_t len = i - elStart;
            // copy number to buffer for atoi()
            strncpy(numberBuffer, &valuesLine[elStart], len);
            elStart = i + 1;  // continue after comma
            numberBuffer[len] = 0;
            elements[elIndex++] = atof(numberBuffer);
        }
    }

    vec->data = elements;
    vec->length = elementCount;

    return 0;
}
