#define CUDA /* Tell the preprocessor this is a CUDA/C++ file */

#include "matrix-utils.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS_PER_BLOCK 1024

__global__ void __dot_product(double* a, double* b, double* c,
                              size_t n, size_t p, size_t m){
    size_t index = threadIdx.x + blockIdx.x*blockDim.x;

    if(index >= n*m) return;

    double v = 0.0;
    size_t i = index/m, j = index%m;

    for(size_t offset = 0; offset<p; offset++){
        v += *(a + i*p + offset) * *(b + m*offset + j);
    }

    c[index] = v;
}

extern "C" MATRIX dot_product(double* a, double* b, size_t n, size_t p, size_t m){
    double *da, *db, *dc;
    double* c = (double*)malloc(sizeof(double)*n*m);
    if(c==NULL) {
        perror("CUDA: while allocating memory");
        exit(EXIT_FAILURE);
    }

    int blocks = (n*m)/MAX_THREADS_PER_BLOCK + 1;

    cudaMalloc((void**)&da, n*p*sizeof(double));
    cudaMalloc((void**)&db, p*m*sizeof(double));
    cudaMalloc((void**)&dc, n*m*sizeof(double));

    cudaMemcpy(da, a, n*p*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(db, b, p*m*sizeof(double), cudaMemcpyHostToDevice);

    __dot_product<<<blocks,MAX_THREADS_PER_BLOCK>>>(da, db, dc, n, p, m);
    cudaError_t err;
    if((err=cudaDeviceSynchronize())!=cudaSuccess){
    }

    cudaMemcpy(c, dc, n*m*sizeof(double), cudaMemcpyDeviceToHost);
    MATRIX mat = {.data=c, .n=n, .m=m};

    cudaFree(da); cudaFree(db); cudaFree(dc);

    return mat;
}

