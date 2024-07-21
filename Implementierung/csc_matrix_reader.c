#include "csc_matrix_reader.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Note:
 * In cases of multiple commas or spaces between values the function will ignore
 * the extra commas and spaces and filter out the values. "1.0,,,,,1.0" will be
 * read as "1.0,1.0". "1.0,     1.0" will be read as "1.0,1.0". The function
 * will also ignore empty lines. This will later result in an error when reading
 * the matrix data and is thereby handled.
 */

// Enum for identifying the data type of the vector
typedef enum { FLOAT, INT } DataType;

int readDynamicLine(FILE* file, void** vec, size_t* size_ptr, DataType type) {
    char* line = NULL;
    size_t lineSize = 0;
    ssize_t read;

    // Read line from file
    read = getline(&line, &lineSize, file);
    // Check if line was read successfully
    if (read == -1) {
        free(line);
        fprintf(stderr, "Error reading line from file.\n");
        return EXIT_FAILURE;
    }

    // Check for empty lines
    if (lineSize == 1) {
        free(line);
        return EXIT_FAILURE;
    }

    // Initialize vector
    size_t count = 0;
    size_t capacity = 4;
    if (type == FLOAT) {
        *vec = malloc(capacity * sizeof(float));
    } else {
        *vec = malloc(capacity * sizeof(size_t));
    }
    if (*vec == NULL) {
        fprintf(stderr, "Failed to allocate initial memory for vector\n");
        free(line);
        return EXIT_FAILURE;
    }

    char* end;
    // Split line iteratively into tokens
    char* p = strtok(line, ", \t\n");
    while (p != NULL) {
        count++;
        // Reallocate memory if necessary
        if (count > capacity) {
            capacity *= 2;
            if (type == FLOAT) {
                float* temp = realloc(*vec, capacity * sizeof(float));
                if (temp == NULL) {
                    fprintf(stderr, "Failed to reallocate memory for vector\n");
                    free(line);
                    return EXIT_FAILURE;
                }
                *vec = temp;
            } else {
                size_t* temp = realloc(*vec, capacity * sizeof(size_t));
                if (temp == NULL) {
                    fprintf(stderr, "Failed to reallocate memory for vector\n");
                    free(line);
                    return EXIT_FAILURE;
                }
                *vec = temp;
            }
        }
        // Convert token to float or int
        if (type == FLOAT) {
            errno = 0;
            ((float*)(*vec))[count - 1] = strtof(p, &end);
            if (errno == ERANGE || *end != '\0' || p == end) {
                fprintf(stderr,
                        "Invalid float value or number out of range: '%s'\n",
                        p);
                free(line);
                return EXIT_FAILURE;
            }
        } else if (type == INT) {
            errno = 0;
            ((size_t*)(*vec))[count - 1] = strtol(p, &end, 10);
            if (errno == ERANGE || *end != '\0' || p == end) {
                fprintf(stderr,
                        "Invalid integer value or number out of range: '%s'\n",
                        p);
                free(line);
                return EXIT_FAILURE;
            }
        }
        // Get next token
        p = strtok(NULL, ", \t\n");
    }

    // Store size of vector
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
    if (sscanf(line, "%zd,%zd%c", &rows, &cols, &checkChar) != 3 ||
        checkChar != '\n') {
        fprintf(stderr, "Error: Format of dimensions must be 'int,int\\n'.\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    matrix->rows = rows;
    matrix->cols = cols;

    // Read data values
    int res =
        readDynamicLine(file, (void**)&matrix->values, &matrix->nnz, FLOAT);
    if (res != EXIT_SUCCESS) {
        fprintf(stderr, "Error reading matrix data values\n");
        fclose(file);
        return res;
    }

    // Read row indices
    size_t row_indices_length;
    res = readDynamicLine(file, (void**)&matrix->row_indices,
                          &row_indices_length, INT);
    if (res != EXIT_SUCCESS) {
        fprintf(stderr, "Error reading matrix row index values\n");
        fclose(file);
        return res;
    }

    // Read column pointers
    res = readDynamicLine(file, (void**)&matrix->col_ptr,
                          &matrix->col_ptr_length, INT);
    if (res != EXIT_SUCCESS) {
        fprintf(stderr, "Error reading matrix col ptr values\n");
        fclose(file);
        return res;
    }

    // Check if the number of row indices matches the number of non-zero
    // elements
    if (row_indices_length != matrix->nnz) {
        fprintf(stderr,
                "Error: Number of row indices do not match number of non-zero "
                "elements\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Check for extra data in file
    char extraBuffer[10];
    if (fgets(extraBuffer, sizeof(extraBuffer), file) != NULL) {
        fprintf(stderr,
                "Error: Extra data found in file after expected matrix data. "
                "Extra data.\nHint: This error also occurs if you have too "
                "many new lines in you input file.");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Check if the number of columns matches the length of the column pointer
    // array
    if (matrix->cols != matrix->col_ptr_length - 1) {
        fprintf(stderr,
                "Error: Number of columns do not match the length of the "
                "column pointer array\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Ensure all entries in col_ptr for a zero matrix are zero if nnz is 0
    if (matrix->nnz == 0) {
        for (size_t i = 0; i < matrix->col_ptr_length; ++i) {
            if (matrix->col_ptr[i] != 0) {
                fprintf(stderr,
                        "Error: Expected all zero column pointers for an "
                        "all-zero matrix.\n");
                fclose(file);
                return EXIT_FAILURE;
            }
        }
    }

    fclose(file);
    return EXIT_SUCCESS;
}
