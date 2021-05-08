#!/bin/bash

cp -r ./mpi ./input ~/cloud
mpirun -n 12 --hostfile ./hostfile ~/cloud/mpi ./input/mat1.data -v
