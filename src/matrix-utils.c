#include "matrix-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_abort(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

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

DATA read_mat(char* filename){
    DATA d;
    size_t* dims = malloc(sizeof(size_t)*3);

    FILE* data = fopen(filename, "rb");
    fread(dims, sizeof(size_t), 3, data);

    if(ferror(data)){
        char err[100] = "Error reading matrices dims: ";
        strcat(err, filename);
        read_abort(err);
    }

    //Assign matrices dims
    d.n = *dims, d.p = *(dims+1), d.m = dims[2];

    //Read A contents
    d.A = malloc(sizeof(double)*d.n*d.p);
    fread(d.A, sizeof(double), d.n*d.p, data);

    if(ferror(data)){
        char err[100] = "Error reading matrix A: ";
        strcat(err, filename);
        read_abort(err);
    }

    //Read B contents
    d.B = malloc(sizeof(double)*d.m*d.p);
    fread(d.B, sizeof(double), d.m*d.p, data);

    if(ferror(data)){
        char err[100] = "Error reading matrix B: ";
        strcat(err, filename);
        read_abort(err);
    }

    //Read RES contents
    d.RES = malloc(sizeof(double)*d.n*d.m);
    fread(d.RES, sizeof(double), d.n*d.m, data);

    if(ferror(data)){
        char err[100] = "Error reading matrix RES: ";
        strcat(err, filename);
        read_abort(err);
    }

    fclose(data);
    return d;
}

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
