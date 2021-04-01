SHELL := /bin/bash
##
# Parallel matrix multiplication
#
# @file
# @version 0.1

CC = gcc
CFLAGS = -fopenmp -Wall
PAPI_FLAGS = -lpapi
SRC_DIR = ./src
BUILD_DIR = ./build
SRC := $(shell find $(SRC_DIR) -name "*.c")
OBJ := $(patsubst $(SRC_DIR)%.c,$(BUILD_DIR)%.o,$(SRC))
BIN = test

.PHONY: clean

all: directories $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(PAPI_FLAGS) $^ -o $@

directories:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -R $(BUILD_DIR)
	@rm $(BIN)

# end
