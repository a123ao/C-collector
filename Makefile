# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c99 -Wall -Wextra -g -Wno-comment

# Static library name
LIBRARY = build/collector.a

# Source directories
SRC_DIR = src
DST_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(DST_DIR)/%.o)

# Default target
all: $(LIBRARY)

# Create build directory
$(DST_DIR):
	mkdir -p $(DST_DIR)

# Compile source files into object files
$(DST_DIR)/%.o: $(SRC_DIR)/%.c | $(DST_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create static library
$(LIBRARY): $(OBJS)
	ar rcs $@ $(OBJS)

# Clean build files
clean:
	rm -rf $(DST_DIR)

.PHONY: all clean