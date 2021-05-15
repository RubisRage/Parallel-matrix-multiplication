#!/bin/bash

mpirun -n 12 --hostfile ./hostfile ~/cloud/mpi $1 -v
