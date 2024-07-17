#ifndef CSC_MATRIX_MULT_H
#define CSC_MATRIX_MULT_H

#include "./csc_matrix.h"

void matr_mult_csc(const void *a, const void *b, void *result);
void matr_mult_csc_intrinsics(const void *a, const void *b, void *result);

#endif //CSC_MATRIX_MULT_H
