# Default compiler
CC = gcc
CFLAGS = -Wall -I./src/inc
LDFLAGS =

# Cross-compiler
CROSS_CC = arm-linux-gnueabihf-gcc
CROSS_CFLAGS = -Wall -I./src/inc
CROSS_LDFLAGS =

# Define source directories
SRC_DIR_CORE = src/core
SRC_DIR_TEST = src/test
BUILD_DIR = build
CROSS_BUILD_DIR = cross_build

# Define source files
CLIENT_SRC = $(SRC_DIR_CORE)/client.c
MAIN_SRC = $(SRC_DIR_CORE)/main.c
SERVER_SRC = $(SRC_DIR_TEST)/server.c

# Define object files
CLIENT_OBJ = $(BUILD_DIR)/client.o
MAIN_OBJ = $(BUILD_DIR)/main.o
SERVER_OBJ = $(BUILD_DIR)/server.o

# Define cross-compiled object files
CROSS_CLIENT_OBJ = $(CROSS_BUILD_DIR)/client.o
CROSS_MAIN_OBJ = $(CROSS_BUILD_DIR)/main.o
CROSS_SERVER_OBJ = $(CROSS_BUILD_DIR)/server.o

# Define targets
CLIENT_PROGRAM = $(BUILD_DIR)/client_program
SERVER_PROGRAM = $(BUILD_DIR)/server_program

# Define cross-compiled targets
CROSS_CLIENT_PROGRAM = $(CROSS_BUILD_DIR)/client_program
CROSS_SERVER_PROGRAM = $(CROSS_BUILD_DIR)/server_program

# Default target
all: $(CLIENT_PROGRAM) $(SERVER_PROGRAM) $(CROSS_CLIENT_PROGRAM) $(CROSS_SERVER_PROGRAM)

# Build client program
$(CLIENT_PROGRAM): $(MAIN_OBJ) $(CLIENT_OBJ)
	@echo "Building default client program..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(MAIN_OBJ) $(CLIENT_OBJ)

# Build server program
$(SERVER_PROGRAM): $(SERVER_OBJ)
	@echo "Building default server program..."
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SERVER_OBJ)

# Cross-compile client program
$(CROSS_CLIENT_PROGRAM): $(CROSS_MAIN_OBJ) $(CROSS_CLIENT_OBJ)
	@echo "Building cross-compiled client program..."
	$(CROSS_CC) $(CROSS_CFLAGS) $(CROSS_LDFLAGS) -o $@ $(CROSS_MAIN_OBJ) $(CROSS_CLIENT_OBJ)

# Cross-compile server program
$(CROSS_SERVER_PROGRAM): $(CROSS_SERVER_OBJ)
	@echo "Building cross-compiled server program..."
	$(CROSS_CC) $(CROSS_CFLAGS) $(CROSS_LDFLAGS) -o $@ $(CROSS_SERVER_OBJ)

# Compile main program's source files to object files
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	mkdir -p $(BUILD_DIR)
	@echo "Compiling $(MAIN_SRC)..."
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/client.o: $(CLIENT_SRC)
	mkdir -p $(BUILD_DIR)
	@echo "Compiling $(CLIENT_SRC)..."
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile server program's source file to object file
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

# Compile server program's source file to object file for cross-compilation
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
