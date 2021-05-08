#include "matrix-utils.h"
#include <stdio.h>
#include <papi.h>

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
