# Makefile for Raytracer program #

# compiler #
CXX ?= g++

# path #
SRC_PATH   = src
UTIL_PATH  = src/util
BUILD_PATH = build

# executable #
BIN_NAME = raytracer

# extensions #
SRC_EXT = cpp
HED_EXT = h
OBJ_EXT = o
DEP_EXT = d

# code files #
SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)')
HEADERS = $(shell find $(SRC_PATH) -name '*.$(HED_EXT)')
OBJECTS = $(patsubst $(SRC_PATH)/%.$(SRC_EXT), $(BUILD_PATH)/%.$(OBJ_EXT), $(SOURCES))
DEPENDS = $(OBJECTS:%.$(OBJ_EXT)=%.$(DEP_EXT))

# flags #
COMPILE_FLAGS = -std=c++11 -Wall -pthread -g -O3

# arguments #
ARGS = ""

# Clean build
.PHONY: all
all: $(SOURCES) $(HEADERS)
	@echo "Compiling sources and creating executable $(BIN_NAME)..."
	$(CXX) $(SOURCES) $(COMPILE_FLAGS) -o $(BIN_NAME) -I $(UTIL_PATH)
	@echo "Done."

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) and $(BUILD_PATH) directory..."
	rm -rf $(BIN_NAME)
	rm -rf $(BUILD_PATH)
	@echo "Done."

.PHONY: run
run:
	./$(BIN_NAME) $(ARGS)

# This build will create object files and link them, subsequent makes are faster
.PHONY: fast
fast: $(BIN_NAME)
	@echo "Done."

$(BIN_NAME) : $(OBJECTS)
	@echo "Linking object files..."
	@mkdir -p $(BUILD_PATH)
	@$(CXX) $(COMPILE_FLAGS) $^ -o $(BIN_NAME)

-include $(DEPENDS)

$(BUILD_PATH)/%.$(OBJ_EXT) : $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Building $(notdir $@)"
	@mkdir -p $(dir $@)
	@$(CXX) $(COMPILE_FLAGS) -MMD -c $< -I $(UTIL_PATH) -o $@
