#include "matrix-utils.h"
#include <stdio.h>
#include <papi.h>

int main(int argc, char** argv){
    DATA d = read_mat(argv[1]);

    puts("A");
    puts("B");
    PAPI_hl_region_begin("dot-product");
    MATRIX mat = dot_product(d.A, d.B, d.n, d.p, d.m);
    PAPI_hl_region_end("dot-product");
    puts("End.");

    printf("Equal: %d", mat_equals(d.RES, mat.data, mat.n, mat.m));

    destroy_matrix(mat);
    destroy_data(d);
}
