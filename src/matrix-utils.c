#include "matrix-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** Prints error message and exits the program with fail exit code.
*/
void read_abort(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

/*
** Displays the contents of a file as they were double values. The first three size_t sized
** values are ignored.
**
** @param filename - Name of the file to be displayed.
*/
void print_file(char* filename){
    FILE* f = fopen(filename, "rb");

    size_t dim[3];
    fread(dim, 3, sizeof(size_t), f);

    while(!feof(f)){
        double c;
        fread(&c, 1, sizeof(double), f);
        printf("%.2f\n", c);
    }

    fclose(f);
}


/*
** Reads three matrices from a binary file. The first two are example matrices and the third
** must be the dot product of the first two.
**
** @param: filename - Name of the binary file from which the matrices will be read. The file
** is expected to be formated in the following way: Three size_t integers describing the
** matrices dimensions, then the three matrices contiguously.
**
** @return a DATA structure describing all three matrices and their dimensions. More
** information about this structure can be found in the matrix-utils.h
 */
DATA read_mat(char* filename){
    DATA d;
    FILE* data = fopen(filename, "rb");

    // Allocate and read matrix dimensions
    size_t* dims = malloc(sizeof(size_t)*3);
    fread(dims, sizeof(size_t), 3, data);

    if(ferror(data)){
        char err[100] = "Error reading matrices dims: ";
        strcat(err, filename);
        read_abort(err);
    }

    // Assign matrices dims
    d.n = *dims, d.p = *(dims+1), d.m = dims[2];

    // Read A contents
    d.A = malloc(sizeof(double)*d.n*d.p);
    fread(d.A, sizeof(double), d.n*d.p, data);

    if(ferror(data)){
        char err[100] = "Error reading matrix A: ";
        strcat(err, filename);
        read_abort(err);
    }

    // Read B contents
    d.B = malloc(sizeof(double)*d.m*d.p);
    fread(d.B, sizeof(double), d.m*d.p, data);

    if(ferror(data)){
        char err[100] = "Error reading matrix B: ";
        strcat(err, filename);
        read_abort(err);
    }

    // Read RES contents
    d.RES = malloc(sizeof(double)*d.n*d.m);
    fread(d.RES, sizeof(double), d.n*d.m, data);

    if(ferror(data)){
        char err[100] = "Error reading matrix RES: ";
        strcat(err, filename);
        read_abort(err);
    }

    // Close file
    fclose(data);
    return d;
}

/*
** Displays the contents of a double valued matrix.
**
** @param mat - pointer to matrix contents.
** @param n - number of rows of the matrix.
** @param m - number of columns of the matrix.
*/
void print_mat(double* mat, size_t n, size_t m){
    fputs("[\n ", stdout);

    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < m; j++){
            printf("%.2f, ", *(mat+i*m+j));
        }
        if(i != n-1)
            fputs("\n ", stdout);
    }
    puts("\n]");
}

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
int mat_equals(double* a, double* b, size_t n, size_t m){
    if(n!=m) return 0;

    for(size_t i = 0; i < n*m; i++){
        if(a[i] != b[i]) return 0;
    }

    return 1;
}

void destroy_matrix(MATRIX m){
    free(m.data);
}

void destroy_data(DATA d){
   free(d.A);
   free(d.B);
   free(d.RES);
}
