# Makefile for Raytracer program #

# compiler #
CXX ?= g++

# path #
SRC_PATH = src

# executable #
BIN_NAME = raytracer

# extensions #
SRC_EXT = cpp
HED_EXT = h

# code files #
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)')
HEADERS = $(shell find $(SRC_PATH) -name '*.$(HED_EXT)')

# flags #
COMPILE_FLAGS = -std=c++11 -Wall -g #-O3

# arguments #
ARGS = ""

.PHONY: all
all: $(SOURCES) $(HEADERS)
	@echo "Compiling sources and creating executable $(BIN_NAME)..."
	$(CXX) $(SOURCES) $(COMPILE_FLAGS) -o $(BIN_NAME)
	@echo "Done."

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME)..."
	rm -rf $(BIN_NAME)
	@echo "Done."

.PHONY: run
run:
./$(BIN_NAME) $(ARGS)
