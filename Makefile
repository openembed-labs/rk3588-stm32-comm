# Default compiler
CC = gcc
CFLAGS = -Wall -I./src/inc
LDFLAGS =

# Cross-compiler
CROSS_CC = aarch64-linux-gnu-gcc
CROSS_CFLAGS = -Wall -I./src/inc
CROSS_LDFLAGS = -static

# Define source directories
SRC_DIR_CORE = src/core
SRC_DIR_INC = src/inc
BUILD_DIR = build
CROSS_BUILD_DIR = cross_build

# Define source files
CLIENT_SRC = $(SRC_DIR_CORE)/client.c
MAIN_SRC = $(SRC_DIR_CORE)/main.c
SERVER_SRC = $(SRC_DIR_CORE)/server.c

# Define object files
CLIENT_OBJ = $(BUILD_DIR)/client.o
MAIN_OBJ = $(BUILD_DIR)/main.o
SERVER_OBJ = $(BUILD_DIR)/server.o

# Define cross-compiled object files
CROSS_CLIENT_OBJ = $(CROSS_BUILD_DIR)/client.o
CROSS_MAIN_OBJ = $(CROSS_BUILD_DIR)/main.o
CROSS_SERVER_OBJ = $(CROSS_BUILD_DIR)/server.o

# Define targets
COMBINED_PROGRAM = $(BUILD_DIR)/combined_program

# Define cross-compiled targets
CROSS_COMBINED_PROGRAM = $(CROSS_BUILD_DIR)/combined_program

# Default target
all: $(COMBINED_PROGRAM) $(CROSS_COMBINED_PROGRAM)

# Build combined program
$(COMBINED_PROGRAM): $(MAIN_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ)
	@echo "Building default combined program..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(MAIN_OBJ) $(CLIENT_OBJ) $(SERVER_OBJ)

# Cross-compile combined program
$(CROSS_COMBINED_PROGRAM): $(CROSS_MAIN_OBJ) $(CROSS_CLIENT_OBJ) $(CROSS_SERVER_OBJ)
	@echo "Building cross-compiled combined program..."
	$(CROSS_CC) $(CROSS_CFLAGS) $(CROSS_LDFLAGS) -o $@ $(CROSS_MAIN_OBJ) $(CROSS_CLIENT_OBJ) $(CROSS_SERVER_OBJ)

# Compile main program's source files to object files
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	mkdir -p $(BUILD_DIR)
	@echo "Compiling $(MAIN_SRC)..."
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/client.o: $(CLIENT_SRC)
	mkdir -p $(BUILD_DIR)
	@echo "Compiling $(CLIENT_SRC)..."
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/server.o: $(SERVER_SRC)
	mkdir -p $(BUILD_DIR)
	@echo "Compiling $(SERVER_SRC)..."
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile main program's source files to object files for cross-compilation
$(CROSS_BUILD_DIR)/main.o: $(MAIN_SRC)
	mkdir -p $(CROSS_BUILD_DIR)
	@echo "Cross-compiling $(MAIN_SRC)..."
	$(CROSS_CC) $(CROSS_CFLAGS) -c -o $@ $<

$(CROSS_BUILD_DIR)/client.o: $(CLIENT_SRC)
	mkdir -p $(CROSS_BUILD_DIR)
	@echo "Cross-compiling $(CLIENT_SRC)..."
	$(CROSS_CC) $(CROSS_CFLAGS) -c -o $@ $<

$(CROSS_BUILD_DIR)/server.o: $(SERVER_SRC)
	mkdir -p $(CROSS_BUILD_DIR)
	@echo "Cross-compiling $(SERVER_SRC)..."
	$(CROSS_CC) $(CROSS_CFLAGS) -c -o $@ $<

# Clean up build files
clean:
	@echo "Cleaning build directories..."
	rm -f $(BUILD_DIR)/*
	rm -f $(CROSS_BUILD_DIR)/*

# Phony targets
.PHONY: all clean
