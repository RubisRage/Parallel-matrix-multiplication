SHELL := /bin/bash

##
# Parallel matrix multiplication
#
# @file
# @version 0.1

CC = gcc
CFLAGS = -lpapi -g -I$(SRC_DIR)
CUDA_FLAGS = -lpapi -g -G -I$(SRC_DIR)

BIN = cuda cuda_dim omp mpi seq mpi_br
SRC_DIR = ./src

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)%.c,$(BUILD_DIR)%.o,$(SRC))

SEQ_SRC := $(SRC_DIR)/seq/seq.c
CUDA_SRC := $(SRC_DIR)/cuda/cuda-matmul.cu
OPENMP_SRC := $(SRC_DIR)/openmp/openmp-matmul.c
MPI_SRC := $(wildcard $(SRC_DIR)/mpi/*.c)
MPI_br_SRC := $(SRC_DIR)/mpi_br/mpi_br-matmul.c

.PHONY: clean

all: $(BIN)

seq: $(SRC) $(SEQ_SRC)
	$(CC) $(CFLAGS) -DSESSION='"seq"' -o $@ $^

cuda: $(CUDA_SRC) $(SRC)
	nvcc $(CUDA_FLAGS) -DSESSION='"CUDA"' -o $@ $^

cuda_dim: $(CUDA_SRC) $(SRC)
	nvcc $(CUDA_FLAGS) -DSESSION='"CUDA_dim"' -o $@ $^

omp: $(OPENMP_SRC) $(SRC)
	$(CC) $(CFLAGS) -DSESSION='"omp"' -fopenmp -o $@ $(OPENMP_SRC) $(SRC)

mpi: $(MPI_SRC) $(SRC)
	mpicc $(CFLAGS) -Wall -DMPI -o $@ $^

mpi_br: $(MPI_br_SRC) $(SRC)
	mpicc $(CFLAGS) -Wall -DMPI -o $@ $^

clean:
	@rm -f $(BIN)

# end
