# Set the cross-compilation toolchain
# CC = arm-linux-gnueabihf-gcc
CC = gcc
CFLAGS = -Wall -I./src/inc
LDFLAGS =

# Define source directories
SRC_DIR_CORE = src/core
SRC_DIR_TEST = src/test
BUILD_DIR = build

# Define source files
CLIENT_SRC = $(SRC_DIR_CORE)/client.c
MAIN_SRC = $(SRC_DIR_CORE)/main.c
SERVER_SRC = $(SRC_DIR_TEST)/server.c

# Define object files
CLIENT_OBJ = $(BUILD_DIR)/client.o
MAIN_OBJ = $(BUILD_DIR)/main.o
SERVER_OBJ = $(BUILD_DIR)/server.o

# Define targets
CLIENT_PROGRAM = $(BUILD_DIR)/client_program
SERVER_PROGRAM = $(BUILD_DIR)/server_program

# Default target
all: $(CLIENT_PROGRAM) $(SERVER_PROGRAM)

# Build client program
$(CLIENT_PROGRAM): $(MAIN_OBJ) $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(MAIN_OBJ) $(CLIENT_OBJ)

# Build server program
$(SERVER_PROGRAM): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SERVER_OBJ)

# Compile main program's source files to object files
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/client.o: $(CLIENT_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile server program's source file to object file
$(BUILD_DIR)/server.o: $(SERVER_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build files
clean:
	rm -f $(BUILD_DIR)/*

# Phony targets
.PHONY: all clean
