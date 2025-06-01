# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lX11 -lpthread

# Source and output
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
BIN = david_display_manager

# Default target
all: $(BIN)

# Build executable
$(BIN): $(SRC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

# Clean object and binary files
clean:
	rm -f $(BIN) $(SRC_DIR)/*.o

.PHONY: all clean
