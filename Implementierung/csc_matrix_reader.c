#include "csc_matrix_reader.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int readDynamicFloatLine(FILE* file, float** vec, size_t* size_ptr) {
    char* line = NULL;
    size_t lineSize = 0;
    ssize_t read;

    read = getline(&line, &lineSize, file);
    if (read == -1) {
        fprintf(stderr, "Error reading line from file.\n");
        return EXIT_FAILURE;
    }

    if (lineSize == 1) { // Check for empty lines - New Check
        free(line);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    char* end;
    for (char* p = strtok(line, ", \t\n"); p != NULL; p = strtok(NULL, ", \t\n")) {
        count++;
        *vec = realloc(*vec, count * sizeof(float));
        if (*vec == NULL) {
            fprintf(stderr, "Failed to allocate memory for vector\n");
            free(line);
            return EXIT_FAILURE;
        }
        errno = 0;
        (*vec)[count - 1] = strtof(p, &end);
        if (errno == ERANGE || *end != '\0') {
            fprintf(stderr, "Invalid float value or number out of range: '%s'\n", p);
            //free(*vec);
            free(line);
            return EXIT_FAILURE;
        }
    }

    *size_ptr = count;
    free(line);
    return EXIT_SUCCESS;
}

int readDynamicIntLine(FILE* file, size_t** vec, size_t* size_ptr) {
    char* line = NULL;
    size_t lineSize = 0;
    ssize_t read;

    read = getline(&line, &lineSize, file);
    if (read == -1) {
        fprintf(stderr, "Error reading line from file.\n");
        return EXIT_FAILURE;
    }

    if (lineSize == 1) { // Check for empty lines - New Check
        free(line);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    char* end;
    for (char* p = strtok(line, ", \t\n"); p != NULL; p = strtok(NULL, ", \t\n")) {
        count++;
        *vec = realloc(*vec, count * sizeof(size_t));
        if (*vec == NULL) {
            fprintf(stderr, "Failed to allocate memory for vector\n");
            free(line);
            return EXIT_FAILURE;
        }
        errno = 0;
        (*vec)[count - 1] = strtol(p, &end, 10);
        if (errno == ERANGE || *end != '\0') {
            fprintf(stderr, "Invalid integer value or number out of range: '%s'\n", p);
            //free(*vec);
            free(line);
            return EXIT_FAILURE;
        }
    }

    *size_ptr = count;
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

    // Read dimensions
    char line[1000];
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error reading line from file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    size_t rows, cols;
    char checkChar;
    if (sscanf(line, "%zd,%zd%c", &rows, &cols, &checkChar) != 3 || checkChar != '\n') {
        fprintf(stderr, "Error: Format of dimensions must be 'int,int\\n'.\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    matrix->rows = rows;
    matrix->cols = cols;

    // Read data values
    int res = readDynamicFloatLine(file, &matrix->values, &matrix->nnz);
    if (res != EXIT_SUCCESS) {
        fprintf(stderr, "Error reading matrix data values\n");
        fclose(file);
        return res;
    }

    // Read row indices
    size_t row_indices_length;
    res = readDynamicIntLine(file, &matrix->row_indices, &row_indices_length);
    if (res != EXIT_SUCCESS) {
        //free(matrix->values);
        fprintf(stderr, "Error reading matrix row index values\n");
        fclose(file);
        return res;
    }

    // Read column pointers
    res = readDynamicIntLine(file, &matrix->col_ptr, &matrix->col_ptr_length);
    if (res != EXIT_SUCCESS) {
        //free(matrix->values);
        //free(matrix->row_indices);
        fprintf(stderr, "Error reading matrix col ptr values\n");
        fclose(file);
        return res;
    }


    // Check if the number of row indices matches the number of non-zero elements
    if (row_indices_length != matrix->nnz) {
        fprintf(stderr, "Error: Number of row indices do not match number of non-zero elements\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    //Check for extra data in file
    char extraBuffer[10];
    if (fgets(extraBuffer, sizeof(extraBuffer), file) != NULL) {
        fprintf(stderr, "Error: Extra data found in file after expected matrix data. Extra data.\nHint: This error also occurs if you have too many new lines in you input file.");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Check if the number of columns matches the length of the column pointer array
    if (matrix->cols != matrix->col_ptr_length - 1) {
        fprintf(stderr, "Error: Number of columns do not match the length of the column pointer array\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Ensure all entries in col_ptr for a zero matrix are zero if nnz is 0
    if (matrix->nnz == 0) {
        for (size_t i = 0; i < matrix->col_ptr_length; ++i) {
            if (matrix->col_ptr[i] != 0) {
                fprintf(stderr, "Error: Expected all zero column pointers for an all-zero matrix.\n");
                fclose(file);
                return EXIT_FAILURE;
            }
        }
    }

    fclose(file);
    return EXIT_SUCCESS;
}