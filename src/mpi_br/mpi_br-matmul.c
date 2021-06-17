#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <papi.h>
#include "matrix-utils.h"

/*
** Function used to share the workload among all processes. Process with
** rank 0 acts as a master dividing work and sharing the sending the
** previously read data to each process. In this version the data sent
** is one of matrix A rows and the complete matrix B.
**
** @param d For process with rank 0 the DATA structure containing the
** information needed to compute the multiplication. For any other process
** a DATA structure that will be initialized with the mentioned information.
** @param rank Integer indication current process rank.
** @param world_size Integer indication the total number of processes.
** @param start Integer pointer in which will be stored the beginning of
** range of elements that the current process has to compute.
** @param end Integer pointer in which will be stored the index of the last
** element, not included, that the current process has to compute.
*/
void share_work(DATA* d, int rank, int world_size, int* start, int* end){
    int dim[3];

    if(rank==0){
        dim[0] = d->n; dim[1] = d->p; dim[2] = d->m;
    }

    MPI_Bcast(dim, 3, MPI_INT, 0, MPI_COMM_WORLD);

    /* Master process */
    if(rank==0){
        int nrow = d->n;
        int base_chunk = nrow/world_size;
        int remainder =  nrow%world_size;
        int offset;
        *start = 0;

        if(remainder == 0){
            offset = *end = base_chunk;
        } else {
            offset = *end = base_chunk+1;
        }

        for(int i = 1; i < world_size; i++){
            int range[2] = {offset, offset+base_chunk};
            offset += base_chunk;

            if(i < remainder){
                range[1]++;
                offset++;
            }

            MPI_Send(range, 2, MPI_INT, i, 0, MPI_COMM_WORLD);     /* Send assigned range of rows */
            MPI_Send(d->A+range[0]*d->p, (range[1]-range[0])*d->p, /* Send assigned rows */
                     MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

    /* Slave process */
    }else{
        int range[2];
        MPI_Recv(range, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        *start = range[0];
        *end = range[1];
        d->n = dim[0]; d->p = dim[1]; d->m = dim[2];
        d->A = malloc((*end-*start) * d->p * sizeof(double)); /* Allocate space for assigned rows */
        d->B = malloc(d->p * d->m * sizeof(double));

        MPI_Recv(d->A, (*end-*start)*d->p, MPI_DOUBLE, 0, 0,  /* Recive assigned rows */
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Bcast(d->B, d->p*d->m, MPI_DOUBLE, 0, MPI_COMM_WORLD); /* Broadcast matrix B */
}

/*
** Computes a portion of the matrix multiplication a*b. The portion computed is the rows in the range
** [start, end).
**
** @param a Pointer to a subsegment of matrix a rows.
** @param b Pointer to the elements matrix b.
** @param n Number of rows of a.
** @param p Number of columns of a and rows of b.
** @param m Number of columns of b.
** @param start Index to the first element in the range of the elements to be computed.
** @param end Index to the last element in the range of the elements to be computed, not included.
**
** @return Pointer to the result of the partial matrix multiplication.
 */
double* __dot_product(double* a, double* b, size_t n, size_t p, size_t m, int start, int end){
    double* c = malloc(n*m*sizeof(double));
    if(c == NULL){
        perror("Error creating tmp array to contain matrix multiplication partial results");
        abort();
    }

    for(int i = start; i < end; i++){
        for(int j = 0; j < m; j++){
            double v = 0.0;

            for(int offset = 0; offset < p; offset++){
                v += a[(i-start)*p + offset] * b[offset*m + j];
            }

            c[(i-start)*m + j] = v;
        }
    }

    return c;
}

/*
** Collect matrix multiplication results back to the process with rank 0.
**
** @param c double pointer to the partially computed matrix multiplication
** @param csize Total number of elements of the complete matrix c
** @param rank Rank of the process
** @param world_size Total number of processes.
** @param start Index pointing to the beginning of the subsection of the result matrix that
** the current process has computed.
** @param start Index pointing to the ending of the subsection of the result matrix that
** the current process has computed.
**
** @return Double pointer referring the result of the matrix multiplication.
*/
double* collect_outcome(double* c, DATA d, int rank, int world_size, int start, int end){

    size_t csize = d.n*d.m;

    if(rank==0){
        c = realloc(c, csize*sizeof(double));
        if(c == NULL){
            perror("Not enough memory to store multiplication's outcome.");
            abort();
        }

        size_t offset = end*d.m;

        for(int i = 1; i < world_size; i++){
            MPI_Status status;
            int step;

            MPI_Recv(c+offset, csize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_DOUBLE, &step);

            offset += step;
        }

        return c;
    }else {
        MPI_Send(c, (end-start)*d.m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        return NULL;
    }
}

/*
** Computes the matrix multiplication between the two matrices passed.
**
** @param d DATA structure containing matrices A and B.
** @param rank Rank of the current process.
** @param world_size Total number of processes.
*/
MATRIX dot_product(DATA d, int rank, int world_size){
    int start, end;
    PAPI_hl_region_begin("send-mat");
    share_work(&d, rank, world_size, &start, &end);
    PAPI_hl_region_end("send-mat");

    double* c = __dot_product(d.A, d.B, d.n, d.p, d.m, start, end);

    MATRIX m = {.data=NULL, .n=0, .m=0};

    if(rank == 0){
        m.n = d.n;
        m.m = d.m;
        PAPI_hl_region_begin("recv-mat");
        m.data = collect_outcome(c, d, rank, world_size, start, end);
        PAPI_hl_region_end("recv-mat");
    } else {
        collect_outcome(c, d, rank, world_size, start, end);
        free(c);
        destroy_data(d);
    }

    return m;
}
