#include "csc_matrix_reader.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readFloatLine(FILE* file, float** vec, size_t* size_ptr) {
    // Constants
    size_t maxLineLength = 100;
    const size_t maxNumberLength = 10;

    // Read Line
    char* line = malloc(sizeof(char)*maxLineLength);
    if (line == NULL) {
        fprintf(stderr, "Failed to allocate memory for line buffer\n");
        return EXIT_FAILURE;
    }

    size_t lineSize = getline(&line, &maxLineLength, file);
    if (lineSize == 0) {
        free(line);
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
    if (elementBuffer == NULL) {
        free(line);
        fprintf(stderr, "Failed to allocate memory for element buffer\n");
        return EXIT_FAILURE;
    }
    // Address of the current position in the element buffer
    size_t elementIndex = 0;

    // Position of the start of the next element in the line buffer
    size_t elementStart = 0;

    // Necessary for null terminated strings (string parsing)
    char numberBuffer[maxNumberLength];

    for (size_t i = 0; line[i] != 0; i++) {
        // Current char
        char c = line[i];
        if (c == ',' || c == '\n' || c == 0) {
            // length of the current number string slice slice
            size_t len = i - elementStart;
            // copy the string slice into the numberBuffer
            strncpy(numberBuffer, &line[elementStart], len);
            // null terminate the numberBuffer for string things
            numberBuffer[len] = 0;

            // convert to number
            char* end;
            elementBuffer[elementIndex++] = strtof((char*)&numberBuffer, &end);

            // check for error
            if (*end != 0) {
                fprintf(stderr, "String i tried to read: <%s>\n", numberBuffer);
                free(elementBuffer);
                free(line);
                fprintf(stderr, "Error reading row values: parsing number\n");
                return EXIT_FAILURE;
            }

            // the new elementStart is the Position *after* the comma
            elementStart = i + 1;
        }
    }

    *vec = elementBuffer;
    if (size_ptr != NULL) {
        *size_ptr = elementCount;
    }
    free(line);
    return EXIT_SUCCESS;
}

int readIntLine(FILE* file, size_t** vec, size_t* size_ptr) {
    // Constants
    size_t maxLineLength = 100;
    const size_t maxNumberLength = 10;

    // Read Line
    char* line = malloc(sizeof(char)*maxLineLength);
    if (line == NULL) {
        fprintf(stderr, "Failed to allocate memory for line buffer\n");
        return EXIT_FAILURE;
    }
    size_t lineSize = getline(&line, &maxLineLength, file);
    if (lineSize == 0) {
        free(line);
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
    size_t* elementBuffer = malloc(sizeof(uint64_t) * elementCount);
    if (elementBuffer == NULL) {
        free(line);
        fprintf(stderr, "Failed to allocate memory for element buffer\n");
        return EXIT_FAILURE;
    }
    // Address of the current position in the element buffer
    size_t elementIndex = 0;

    // Position of the start of the next element in the line buffer
    size_t elementStart = 0;
    //Save len

    // Necessary for null terminated strings (string parsing)
    char numberBuffer[maxNumberLength];
    
    size_t i = -1;
    do {
        i++;
        // Current char
        char c = line[i];
        if (c == ',' || c == '\n' || c == 0) {
            // length of the current number string slice slice
            size_t len = i - elementStart;
            // copy the string slice into the numberBuffer
            strncpy(numberBuffer, &line[elementStart], len);
            // null terminate the numberBuffer for string things
            numberBuffer[len] = 0;

            // convert to number
            char* end;
            elementBuffer[elementIndex++] = strtol((char*)&numberBuffer, &end, 10);

            // check for error
            if (*end != 0) {
                fprintf(stderr, "String i tried to read: <%s>\n", numberBuffer);
                free(line);
                free(elementBuffer);
                fprintf(stderr, "Error reading row values: parsing number\n");
                return EXIT_FAILURE;
            }

            // the new elementStart is the Position *after* the comma
            elementStart = i + 1;
        }
    } while(line[i] != 0);

    *vec = elementBuffer;
    if (size_ptr != NULL) {
        *size_ptr = elementCount;
    }
    free(line);
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
    if (fscanf(file, "%zd,%zd\n", &matrix->rows, &matrix->cols) != 2) {
        fprintf(stderr, "Error reading matrix dimensions from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    int res = readFloatLine(file, &matrix->values, &matrix->nnz);
    if (res != EXIT_SUCCESS) {
        fprintf(stderr, "Error reading matrix data values\n");
        fclose(file);
        return res;
    }

    res = readIntLine(file, &matrix->row_indices, NULL);
    if (res != EXIT_SUCCESS) {
        //free(matrix->values);
        fprintf(stderr, "Error reading matrix row index values\n");
        fclose(file);
        return res;
    }

    res = readIntLine(file, &matrix->col_ptr, &matrix->col_ptr_length);
    if (res != EXIT_SUCCESS) {
        //free(matrix->values);
        //free(matrix->row_indices);
        fprintf(stderr, "Error reading matrix col ptr values\n");
        fclose(file);
        return res;
    }

    fclose(file);
    return EXIT_SUCCESS;
}