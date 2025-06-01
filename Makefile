# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lX11 -lpthread

# Directories
SRC_DIR = src
BUILD_DIR = build

# Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
BIN = $(BUILD_DIR)/david_display_manager

# Default target
all: $(BIN)

# Ensure build directory exists, then compile
$(BIN): $(SRC_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

# Clean all build output
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
