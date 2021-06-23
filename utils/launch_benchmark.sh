#!/bin/bash

# Crear tests

tests="test250 test500 test750 test1 test1.5 test2 test2.5 test3 test3.5 test4 test4.5"

make all
cp mpi_br ~/cloud
cp mpi ~/cloud

for n in {1..7};do
    for t in $tests;do
        echo output/seq/$t/seq-$t-$n.json
        ./seq input/$t.data
        mkdir -p output/seq/$t/
        mv papi_hl_output/* output/seq/$t/seq-$t-$n.json
        rmdir papi_hl_output
    done

    for t in $tests;do
        echo output/omp/$t/omp-$t-$n.json
        ./omp input/$t.data
        mkdir -p output/omp/$t/
        mv papi_hl_output/* output/omp/$t/omp-$t-$n.json
        rmdir papi_hl_output
    done

    for t in $tests;do
        echo output/cuda/$t/cuda-$t-$n
        ./cuda input/$t.data
        mkdir -p output/cuda/$t/
        mv papi_hl_output/* output/cuda/$t/cuda-$t-$n.json
        rmdir papi_hl_output
    done

    for t in $tests;do
        echo output/mpi/$t/mpi-$t-$n.json
        mpirun -n 12 --hostfile ./hostfile ~/cloud/mpi input/$t.data -v
        mkdir -p output/mpi/$t/
        mv papi_hl_output/*0.json output/mpi/$t/mpi-$t-$n.json
        rm -R papi_hl_output
    done


    for t in $tests;do
        echo output/mpi_br/$t/mpi_br-$t-$n.json
        mpirun -n 12 --hostfile ./hostfile ~/cloud/mpi_br input/$t.data -v
        mkdir -p output/mpi_br/$t/
        mv papi_hl_output/*0.json output/mpi_br/$t/mpi_br-$t-$n.json
        rm -R papi_hl_output
    done

    for t in $tests;do
        echo output/cuda_dim/$t/cuda_dim-$t-$n
        ./cuda_dim input/$t.data
        mkdir -p output/cuda_dim/$t/
        mv papi_hl_output/* output/cuda_dim/$t/cuda_dim-$t-$n.json
        rmdir papi_hl_output
    done
done
