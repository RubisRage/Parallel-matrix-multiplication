#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <papi.h>
#include <mpi.h>

#include "matrix-utils.h"


#ifdef MPI // If compiled with -DMPI set up multicomputer program

//Function prototype for MPI programs
MATRIX dot_product(DATA d, int rank, int size);

/*
** MPI main function.
*/
int main(int argc, char** argv){
    MPI_Init(NULL,NULL);

    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    DATA d;
    if(rank==0){
        d = read_mat(argv[1]);
    }

    PAPI_hl_region_begin("MPI");
    MATRIX mat = dot_product(d, rank, world_size);
    PAPI_hl_region_end("MPI");

    if(rank==0){
        printf("Output: %s\n", mat_equals(d.RES, mat.data, mat.n, mat.m)==1? "OK" : "ERROR");
        destroy_data(d);
        destroy_matrix(mat);
    }


    MPI_Finalize();
    return EXIT_SUCCESS;
}

#else // If not set up multicore/gpu program

/*
** OpenMP/CUDA main function:
**
** SESSION macro must be defined on compilation time.
*/
int main(int argc, char** argv){
    if(argc > 2){
        fprintf(stderr, "Not enough arguments.\n");
    }

    DATA d = read_mat(argv[1]);

    PAPI_hl_region_begin(SESSION);
    MATRIX mat = dot_product(d.A, d.B, d.n, d.p, d.m);
    PAPI_hl_region_end(SESSION);

    printf("Output: %s\n", mat_equals(d.RES, mat.data, mat.n, mat.m)==1? "OK" : "ERROR");

    destroy_matrix(mat);
    destroy_data(d);
}

#endif /* MPI session check */
