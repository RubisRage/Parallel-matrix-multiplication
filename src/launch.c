#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <papi.h>
#include <mpi.h>

#include "matrix-utils.h"

#ifdef MPI // If compiled with -DMPI set up multicomputer program

MATRIX dot_product(DATA d, int rank, int size);

// MPI main function
int main(int argc, char** argv){
    MPI_Init(NULL,NULL);

    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    DATA d;
    if(rank==0){
        d = read_mat(argv[1]);
    }

    PAPI_hl_region_begin("dot-product");
    MATRIX m = dot_product(d, rank, world_size);
    PAPI_hl_region_end("dot-product");

    MPI_Finalize();
}

#else // If not set up multicore/gpu program

// OpenMP/CUDA main function
int main(int argc, char** argv){
    DATA d = read_mat(argv[1]);
    int pflag = 0;

    if(argc >= 3){
        if(argv[2][0] == 'p') pflag = 1;
    }

    PAPI_hl_region_begin("dot-product");
    MATRIX mat = dot_product(d.A, d.B, d.n, d.p, d.m);
    PAPI_hl_region_end("dot-product");

    printf("Output: %s\n", mat_equals(d.RES, mat.data, mat.n, mat.m)==1? "OK" : "ERROR");
    if(pflag){
        print_mat(d.RES, d.n, d.m);
        print_mat(mat.data, mat.n, mat.m);
    }

    destroy_matrix(mat);
    destroy_data(d);
}

#endif /* MPI session check */
