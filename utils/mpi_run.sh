#!/usr/bin/env bash

cp ./mpi ~/cloud
mpirun -n 12 --hostfile ./hostfile ~/cloud/mpi $1 -v
