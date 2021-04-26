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
BIN = cuda cuda-d openmp

CUDA_SRC := $(SRC_DIR)/cuda/cuda-matmul.cu
CUDA_OBJ := $(BUILD_DIR)/cuda/cuda-matmul.o
OPENMP_SRC := $(SRC_DIR)/openmp/openmp-matmul.c
OPENMP_OBJ := $(BUILD_DIR)/openmp/openmp-matmul.o

.PHONY: clean directories

directories:
	@mkdir -p $(BUILD_DIR)/cuda
	@mkdir -p $(BUILD_DIR)/openmp

cuda: directories | $(CUDA_OBJ) $(OBJ)
	nvcc $(CUDA_FLAGS) -o $@ $(CUDA_OBJ) $(OBJ)

# CUDA debuggleable binary
cuda-d: directories | $(CUDA_SRC) $(SRC)
	nvcc $(CUDA_FLAGS) -o $@ $(CUDA_SRC) $(SRC)

openmp: directories | $(OPENMP_OBJ) $(OBJ)
	$(CC) $(CFLAGS) -fopenmp -o $@ $(OPENMP_OBJ) $(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CUDA_OBJ): $(CUDA_SRC)
	nvcc $(CUDA_FLAGS) -c $< -o $@

$(OPENMP_OBJ): $(OPENMP_SRC)
	$(CC) $(CFLAGS) -fopenmp -c $< -o $@


clean:
	@rm -f -R $(BUILD_DIR) $(BIN)

# end
