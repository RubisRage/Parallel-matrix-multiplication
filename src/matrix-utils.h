#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdlib.h> // Needed for size_t declaration

// Structure declarations

/*
** Structure defined to store the contents of a sample file. A, B and RES
** point to the matrices defined in the file. A and B are example matrices
** and RES is the dot product of A and B.
** The values n, p and m are the dimensions of the matrices specified as
** follows: A(n,p), B(p,m) and RES(n,m)
*/
typedef struct
{
    double* A, *B, *RES;
    size_t n, p, m;
} DATA;

/*
** Structure defined to hold the data of a double precision floating point
** matrix. The data variable points to the matrix contents and the variables
** n and m hold it's dimensions.
*/
typedef struct
{
    double* data;
    size_t n,m;
} MATRIX;

// Function prototypes

/*
** Computes the dot product of matrices A and B concurrently. This function is implemented
** on different translation units based on the technology used: OpenMP, Cuda, OpenMPI.
**
** @param A - pointer to A matrix contents.
** @param B - pointer to B matrix contents.
** @param n - A's number of rows.
** @param p - A's number of columns and B's number of rows.
** @param m - B's number of columns.
**
** @return MATRIX structure holding the data of A and B's dot product. More information
** about this structure can be found on the matrix-utils.h file.
*/
MATRIX dot_product(double* A, double* B, size_t n, size_t p, size_t m);


/*
** Reads three matrices from a binary file. The first two are example matrices and the third
** must be the dot product of the first two.
**
** @param: filename - Name of the binary file from which the matrices will be read. The file
** is expected to be formated in the following way: Three size_t integers describing the
** matrices dimensions, then the three matrices contiguously.
**
** @return a DATA structure describing all three matrices and their dimensions. More
** information about this structure can be found in the matrix-utils.h file.
 */
DATA read_mat(char* dataname);


/*
** Displays the contents of a double valued matrix.
**
** @param mat - pointer to matrix contents.
** @param n - number of rows of the matrix.
** @param m - number of columns of the matrix.
*/
void print_mat(double* mat, size_t n, size_t m);


/*
** Displays the contents of a file as they were double values. The first three size_t sized
** values are ignored.
**
** @param filename - Name of the file to be displayed.
*/
void print_file(char*);


/*
** Checks for equality between two matrices.
**
** @param a - pointer to first matrix.
** @param b - pointer to second matrix.
** @param n - number of rows of both matrices.
** @param m - number of columns of both matrices.
**
** @return 1 if both matrices are equal, 0 otherwise.
*/
int mat_equals(double* a, double* b, size_t n, size_t m);

void destroy_matrix(MATRIX);

void destroy_data(DATA);

#endif /* MATRIX_UTILS_H */
