#ifndef CSC_MATRIX_MULT_H_V3
#define CSC_MATRIX_MULT_H_V3

#include "./csc_matrix.h"

void matr_mult_csc_V3(const void *a, const void *b, void *result);
void csc_to_csr_V3(const void* m);

#endif //CSC_MATRIX_MULT_H_V3
