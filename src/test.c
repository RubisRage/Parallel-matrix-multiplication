#include "matrix-utils.h"
#include <stdio.h>

int main(){
    DATA d = read_mat("data/mat2.data");

    puts("A");
    print_mat(d.A, d.n, d.p);
    puts("B");
    print_mat(d.B, d.p, d.m);
    MATRIX mat = dot_product(d.A, d.B, d.n, d.p, d.m);
    puts("RES");
    print_mat(d.RES, d.n, d.m);
    puts("NEW");
    print_mat(mat.data, mat.n, mat.m);

    printf("Equal: %d", mat_equals(d.RES, mat.data, mat.n, mat.m));

    destroy_matrix(mat);
    destroy_data(d);
}
