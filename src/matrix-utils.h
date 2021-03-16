#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdlib.h>

typedef struct
{
    double* A, *B, *RES;
    size_t n, p, m; // A(n*p), B(p*m) and RES(n*p)
} DATA;

typedef struct
{
    double* data;
    size_t n,m;
} MATRIX;

MATRIX dot_product(double* A, double* B, size_t n, size_t p, size_t m);
DATA read_mat(char* dataname);
void print_mat(double* mat, size_t n, size_t m);
void print_file(char*);

#endif /* MATRIX_UTILS_H */
