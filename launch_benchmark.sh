#!/bin/bash

# Crear tests



tests="test1.data test2.data test3.data test4.data" # test3.data test4.data"

for t in $tests;do
    echo output/seq-${t:0:5}-3
    ./seq input/$t
    mv papi_hl_output output/seq-${t:0:5}-3
done

for t in $tests;do
    echo output/omp-${t:0:5}-3
    ./omp input/$t
    mv papi_hl_output output/omp-${t:0:5}-3
done

for t in $tests;do
    echo output/cuda-${t:0:5}-2
    ./cuda input/$t
    mv papi_hl_output output/cuda-${t:0:5}-2
done

for t in $tests;do
    echo output/mpi-${t:0:5}-2
    mpirun -n 12 --hostfile ./hostfile ~/cloud/mpi input/$t -v
    mv papi_hl_output output/mpi-${t:0:5}-2
done

# spd-say hello
