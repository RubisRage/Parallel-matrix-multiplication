#!/bin/bash

# Crear tests



# tests="test1 test2 test3 test4" # test3.data test4.data"
tests="test250 test500 test750 test1 test2 test3 test4"

for n in 5;do
    for t in $tests;do
        echo output/seq/$t/seq-$t-$n.json
        ./seq input/$t.data
        mv papi_hl_output/* output/seq/$t/seq-$t-$n.json
        rmdir papi_hl_output
    done

    for t in $tests;do
        echo output/omp/$t/omp-$t-$n.json
        ./omp input/$t.data
        mv papi_hl_output/* output/omp/$t/omp-$t-$n.json
        rmdir papi_hl_output
    done

    for t in $tests;do
        echo output/cuda/$t/cuda-$t-$n.json
        ./cuda input/$t.data
        mv papi_hl_output/* output/cuda/$t/cuda-$t-$n.json
        rmdir papi_hl_output
    done

    for t in $tests;do
        echo output/mpi/$t/mpi-$t-$n.json
        mpirun -n 12 --hostfile ./hostfile ~/cloud/mpi input/$t.data -v
        mv papi_hl_output/*0.json output/mpi/$t/mpi-$t-$n.json
        rm -R papi_hl_output
    done
done

# spd-say hello
