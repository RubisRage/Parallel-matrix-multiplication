SHELL := /bin/bash

##
# Parallel matrix multiplication
#
# @file
# @version 0.1

CC = gcc
SRC_DIR = ./src
BUILD_DIR = ./build
CFLAGS = -lpapi -g -I$(SRC_DIR)
CUDA_FLAGS = -lpapi -g -G -I$(SRC_DIR)

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)%.c,$(BUILD_DIR)%.o,$(SRC))
BIN = cuda omp mpi

CUDA_SRC := $(SRC_DIR)/cuda/cuda-matmul.cu
CUDA_OBJ := $(BUILD_DIR)/cuda/cuda-matmul.o
OPENMP_SRC := $(SRC_DIR)/openmp/openmp-matmul.c
OPENMP_OBJ := $(BUILD_DIR)/openmp/openmp-matmul.o
MPI_SRC := $(wildcard $(SRC_DIR)/mpi/*.c) $(SRC)

.PHONY: clean

directories:
	@mkdir -p $(BUILD_DIR)/openmp


# CUDA debuggleable binary
cuda: $(CUDA_SRC) $(SRC)
	nvcc $(CUDA_FLAGS) -DSESSION='"CUDA"' -o $@ $(CUDA_SRC) $(SRC)

omp: directories | $(OPENMP_OBJ) $(OBJ)
	$(CC) $(CFLAGS) -DSESSION='"omp"' -fopenmp -o $@ $(OPENMP_OBJ) $(OBJ)

mpi: $(MPI_SRC)
	mpicc $(CFLAGS) -lm -Wall -DMPI $(MPI_SRC)  -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -DSESSION='"omp"' -c $< -o $@

$(CUDA_OBJ): $(CUDA_SRC)
	nvcc $(CUDA_FLAGS) -c $< -o $@

$(OPENMP_OBJ): $(OPENMP_SRC)
	$(CC) $(CFLAGS) -DSESSION='"omp"' -fopenmp -c $< -o $@


clean:
	@rm -f -R $(BUILD_DIR) $(BIN)

# end
