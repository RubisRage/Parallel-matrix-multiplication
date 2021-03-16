#include "matrix-utils.h"
#include <omp.h>
#include <stdlib.h>

MATRIX dot_product(double* A, double* B, size_t n, size_t p, size_t m){
    double* res = malloc(sizeof(double)*n*m);

    #pragma omp parallel for collapse(2)
    for(size_t i = 0; i < n; i++){
        for(size_t j = 0; j < m; j++){
            double v = 0.0;

            for(size_t offset = 0; offset < p; offset++){
                v += *(A + i*p + offset) * *(B + offset*m + j);
            }

            *(res + i*m + j) = v;
        }
    }

    MATRIX mat = {.data=res, .n=n, .m=m};
    return mat;
}
