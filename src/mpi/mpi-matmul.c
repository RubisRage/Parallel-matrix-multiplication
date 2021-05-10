#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix-utils.h"

void transfere_mat(DATA* d, int rank){
    int dim[3];

    if(rank==0){
        dim[0] = d->n; dim[1] = d->p; dim[2] = d->m;
    }

    MPI_Bcast(dim, 3, MPI_INT, 0, MPI_COMM_WORLD);


    if(rank!=0){
        d->n = dim[0]; d->p = dim[1]; d->m = dim[2];
        d->A = malloc(d->n * d->p * sizeof(double));
        d->B = malloc(d->p * d->m * sizeof(double));
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(d->A, d->n*d->p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(d->B, d->p*d->m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

double* __dot_product(double* a, double* b, size_t n, size_t p, size_t m, int rank, int world_size){




    return NULL;
}

MATRIX dot_product(DATA d, int rank, int world_size){
    transfere_mat(&d, rank);

    size_t psize = d.n*d.m;
    int divider = psize%world_size;
    int step = ceil(psize/(double)world_size);

    if(rank < divider){
        printf("rank: %d - more work: %d\n", rank, rank*step);

    } else {
        printf("rank: %d - less work: %d\n", rank, 0);
    }

    //print_mat(d.A, d.n, d.p);
    MATRIX m = {.data=NULL, .n=0, .m=0};
    return m;
}
