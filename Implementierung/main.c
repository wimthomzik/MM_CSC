#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <getopt.h>
#include "csc_matrix_datastructure/csc_matrix.h"
#include "csc_matrix_reader/csc_matrix_reader.h"
#include "csc_matrix_writer/csc_matrix_writer.h"

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
                    if (endptr == optarg || errno != 0 || *endptr != '\0' || benchmark < 1) {
                        fprintf(stderr, "Invalid Benchmark: %s\n", optarg);
                        print_usage(progName);
                        return EXIT_FAILURE;
                    }
                } else {
                    // Set default value
                    benchmark = 1;
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
    csc_matrix matrixA, matrixB;

    // Read matrices from input files
    if (readCSCMatrix(inputA, &matrixA) != 0) {
        fprintf(stderr, "Failed to read matrix A\n");
        return EXIT_FAILURE;
    }

    if (readCSCMatrix(inputB, &matrixB) != 0) {
        fprintf(stderr, "Failed to read matrix B\n");
        return EXIT_FAILURE;
    }

    // inputA in matrixA
    // inputB in matrixB
    csc_matrix resultMatrix;

    // Multiply matrices ....

    // Write result matrix to output file
    if (writeCSCMatrix(outputFile, &resultMatrix) != 0) {
        fprintf(stderr, "Failed to write result matrix to %s\n", outputFile);
        return EXIT_FAILURE;
    }
    // Free memory
    free(matrixA.values);
    free(matrixA.row_indices);
    free(matrixA.col_ptr);
    free(matrixB.values);
    free(matrixB.row_indices);
    free(matrixB.col_ptr);

    return 0;
}