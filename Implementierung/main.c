#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <getopt.h>
#include <time.h>
#include "csc_matrix_datastructure/csc_matrix.h"
#include "csc_matrix_reader/csc_matrix_reader.h"
#include "csc_matrix_writer/csc_matrix_writer.h"
#include "csc_matrix_mult/csc_matrix_mult.h"


// Usage messages
const char* usage_msg =
        "Usage: %s -a <filename> -b <filename> -o <filename> [OPTIONS]\n"
        "   or: %s -h             Show help message and optional arguments\n";

// Help message
const char* help_msg =
        "Help Message:\n"
        "Usage: Multiplication of two matrices in CSC format.\n"
        "\n"
        "Optional arguments:\n"
        "  -V <Zahl>      Specify the implementation version to use (default: 0).\n"
        "  -B <Zahl>      Measure and output the runtime of the specified implementation, with an optional number of repetitions.\n"
        "  -a <Dateiname> Input file containing matrix A.\n"
        "  -b <Dateiname> Input file containing matrix B.\n"
        "  -o <Dateiname> Output file for the result matrix.\n"
        "  -h             Display this help message and exit.\n"
        "\nExample:\n"
        "  %s -a matrixA.txt -b matrixB.txt -o result.txt\n"
        "  %s -V 0 -B 10 -a matrixA.txt -b matrixB.txt -o result.txt\n";

// Print usage message
void print_usage(const char* progName) {
    fprintf(stderr, usage_msg, progName, progName, progName);
}

// Print help message
void print_help(const char* progName) {
    fprintf(stderr, help_msg, progName, progName);
}

// Cleanup function
void cleanup(csc_matrix *matrixA, csc_matrix *matrixB, csc_matrix *resultMatrix, int resultAllocated) {
    if (matrixA) {
        free(matrixA->values);
        free(matrixA->row_indices);
        free(matrixA->col_ptr);
    }

    if (matrixB) {
        free(matrixB->values);
        free(matrixB->row_indices);
        free(matrixB->col_ptr);
    }

    if (resultAllocated && resultMatrix) {
        free(resultMatrix->values);
        free(resultMatrix->row_indices);
        free(resultMatrix->col_ptr);
    }
}


int main(int argc, char *argv[]) {

    // Get program name
    const char* progName = argv[0];

    // Check if there are any arguments
    if (argc == 1) {
        print_usage(progName);
        return EXIT_FAILURE;
    }

    // Variables for command line arguments
    char *inputA = NULL, *inputB = NULL, *outputFile = NULL;
    long version = 0, benchmark = 0;

    // Long options
    struct option long_options[] = {
            {"help", no_argument, 0, 'H'},
    };

    // Parse command line arguments
    int opt;
    while ((opt = getopt_long(argc, argv, "V:B:a:b:o:h", long_options, NULL)) != -1)  {
        // Variables for strtol
        char *endptr;
        errno = 0;

        switch (opt) {
            case 'V':
                // Convert string to long
                version = strtol(optarg, &endptr, 10);
                // Check if conversion was successful
                if (endptr == optarg || errno != 0 || *endptr != '\0' || version < 0) {
                    fprintf(stderr, "Invalid Version: %s\n", optarg);
                    print_usage(progName);
                    return EXIT_FAILURE;
                }
                break;
            case 'B':
                // Convert string to long
                if (optarg != NULL) {
                    benchmark = strtol(optarg, &endptr, 10);
                    // Check if conversion was successful
                    if (endptr == optarg || errno != 0 || *endptr != '\0' || benchmark < 3) {
                        if (benchmark < 3) {
                            fprintf(stderr, "Invalid Benchmark: Benchmark has minimum of 3, but was: %s\n", optarg);
                        } else {
                            fprintf(stderr, "Invalid Benchmark: %s\n", optarg);
                        }
                        print_usage(progName);
                        return EXIT_FAILURE;
                    }
                } else {
                    // Set default value
                    benchmark = -1;
                }
                break;
            case 'a':
                inputA = optarg;
                break;
            case 'b':
                inputB = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'h':
            case 'H':
                print_help(progName);
                return EXIT_SUCCESS;
            default:
                print_usage(progName);
                return EXIT_FAILURE;
        }
    }

    // Check if input and output files are set
    if (inputA == NULL || inputB == NULL || outputFile == NULL) {
        fprintf(stderr, "Missing input/output file\n");
        print_usage(progName);
        return EXIT_FAILURE;
    }

    // inputA -> input file containing matrix A
    // inputB -> input file containing matrix B
    // outputFile -> output file for the result matrix
    // version -> specify the implementation version to use (default: 0)
    // benchmark -> measure and output the runtime of the specified implementation, with an optional number of repetitions

    // Read input files

    // Initialize matrices
    csc_matrix matrixA, matrixB, resultMatrix;

    // Read matrices from input files
    if (readCSCMatrix(inputA, &matrixA) != 0) {
        fprintf(stderr, "Failed to read matrix A\n");
        cleanup(&matrixA, &matrixB, &resultMatrix, 0);
        return EXIT_FAILURE;
    }

    if (readCSCMatrix(inputB, &matrixB) != 0) {
        fprintf(stderr, "Failed to read matrix B\n");
        cleanup(&matrixA, &matrixB, &resultMatrix, 0);
        return EXIT_FAILURE;
    }


    if (benchmark >= 3) {
        // Get start time
        struct timespec start;
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 0; i < benchmark; i++) {
            // Multiply matrices based on version set by user
            if (version == 0) {
                matr_mult_csc(&matrixA, &matrixB, &resultMatrix);
            } else if (version == 1) {
                // Implement version 1....
            } else {
                fprintf(stderr, "Unknown version: %ld\n", version);
                cleanup(&matrixA, &matrixB, &resultMatrix, 0);
                return EXIT_FAILURE;
            }
        }

        // Get end time
        struct timespec end;
        clock_gettime(CLOCK_MONOTONIC, &end);
        // Calculate time
        double time = (double) end.tv_sec - (double) start.tv_sec + 1e-9 * (double) (end.tv_nsec - start.tv_nsec);
        double avg_time = time / (double) benchmark;

        // Print benchmark results
        printf("Total time: %f s\n", time);
        printf("Average time: %f ns\n", avg_time);
        if (time < 1.) {
            printf("Warning: Total measurement time is less than one second.\n");
        }
        printf("Warning: Make sure you have exclusive access to resources used.\n");
    } else {
        // Multiply matrices based on version set by user
        if (version == 0) {
            matr_mult_csc(&matrixA, &matrixB, &resultMatrix);
        } else if (version == 1) {
            // Implement version 1....
        } else {
            fprintf(stderr, "Unknown version: %ld\n", version);
            cleanup(&matrixA, &matrixB, &resultMatrix, 0);
            return EXIT_FAILURE;
        }
    }

    // Write result matrix to output file
    if (writeCSCMatrix(outputFile, &resultMatrix) != 0) {
        fprintf(stderr, "Failed to write result matrix to %s\n", outputFile);
        cleanup(&matrixA, &matrixB, &resultMatrix, 1);
        return EXIT_FAILURE;
    }
    // Free memory
    cleanup(&matrixA, &matrixB, &resultMatrix, 1);

    return 0;
}