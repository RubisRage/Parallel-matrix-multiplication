#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "matrix-utils.h"

void share_work(DATA* d, int rank, int world_size, int* start, int* end){
    int dim[3];

    if(rank==0){
        dim[0] = d->n; dim[1] = d->p; dim[2] = d->m;
    }

    MPI_Bcast(dim, 3, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank==0){
        int mat_size = d->n*d->m;
        int base_chunk = mat_size/world_size;
        int remainder = mat_size%world_size;
        int offset;
        *start = 0;

        if(remainder == 0){
            offset = *end = base_chunk;
        } else {
            offset = *end = base_chunk+1;
        }

        for(int i = 1; i < world_size; i++){
            if(i < remainder){
                int range[2] = {offset, offset+base_chunk+1};
                MPI_Send(range, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
                offset += base_chunk+1;
            } else {
                int range[2] = {offset, offset+base_chunk};
                MPI_Send(range, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
                offset += base_chunk;
            }
        }


    }else{
        int range[2];
        MPI_Recv(range, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        *start = range[0];
        *end = range[1];
        d->n = dim[0]; d->p = dim[1]; d->m = dim[2];
        d->A = malloc(d->n * d->p * sizeof(double));
        d->B = malloc(d->p * d->m * sizeof(double));
    }

    MPI_Bcast(d->A, d->n*d->p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(d->B, d->p*d->m, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

double* __dot_product(double* a, double* b, size_t n, size_t p, size_t m, int start, int end){

    double* c = malloc(n*m*sizeof(double));
    if(c == NULL){
        perror("Error creating tmp array to contain matrix multiplication partial results");
        abort();
    }

    for(int index = start; index < end; index++){
        int i = index/m, j=index%m;

        double v = 0.0;
        for(size_t offset = 0; offset < p; offset++){
            v += *(a + i*p + offset) * *(b + m*offset + j);
        }

        c[index] = v;
    }

    return c;
}

double* collect_outcome(double* c, size_t csize, int rank, int world_size, int start, int end){

    if(rank==0){
        int a = 7;
        printf("ID: %d", getpid());
        fflush(stdout);
        while(a);
        c = realloc(c, csize);
        if(c == NULL){
            perror("Not enough memory to store multiplication's outcome.");
            abort();
        }

        size_t offset = end;

        for(int i = 1; i < world_size; i++){
            MPI_Status status;
            int step;

            printf("Dead reciving.\n");
            fflush(stdout);

            MPI_Recv(c+offset, csize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_DOUBLE, &step);

            printf("Nope just kidding\n");
            fflush(stdout);
            offset += step;
        }

        return c;
    }else {
        MPI_Send(c, end-start, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        printf("I did my job\n");
        fflush(stdout);
        return NULL;
    }
}

MATRIX dot_product(DATA d, int rank, int world_size){
    int start, end;
    share_work(&d, rank, world_size, &start, &end);

    double* c = __dot_product(d.A, d.B, d.n, d.p, d.m, start, end);

    MATRIX m = {.data=NULL, .n=0, .m=0};

    if(rank == 0){
        m.n = d.n;
        m.m = d.m;
        m.data = collect_outcome(c, d.n*d.m, rank, world_size, start, end);
    } else {
        collect_outcome(c, d.n*d.m, rank, world_size, start, end);
        free(c);
    }

    return m;
}
