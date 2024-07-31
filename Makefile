# 设置交叉编译工具链
# CC = arm-linux-gnueabihf-gcc
CC = gcc
CFLAGS = -Wall
LDFLAGS =

# 定义源文件和目标文件
MAIN_SRC = src/core/main.c
MAIN_OBJ = build/main.o
MAIN_TARGET = build/client_program

SERVER_SRC = src/test/server.c
SERVER_OBJ = build/server.o
SERVER_TARGET = build/server_program

# 默认目标
all: $(MAIN_TARGET) $(SERVER_TARGET)

# 生成 main 程序的可执行文件
$(MAIN_TARGET): $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(MAIN_OBJ)

# 编译 main 程序的源文件生成目标文件
$(MAIN_OBJ): $(MAIN_SRC)
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

# 生成 server 程序的可执行文件
$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SERVER_OBJ)

# 编译 server 程序的源文件生成目标文件
$(SERVER_OBJ): $(SERVER_SRC)
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

# 清理生成的文件
clean:
	rm -f build/*

# 伪目标
.PHONY: all clean
