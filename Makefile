# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I$(INC_DIR)
DEBUGFLAGS = -Wall -Wextra -g
LDFLAGS = -lz   # link zlib if needed

# Project structure
SRC_DIR = src
INC_DIR = include
RELEASE_DIR = release
DEBUG_DIR = debug

# Source files (can add more later)
# Source files
SRCS = main.c Source/gzip/gzip.c Source/hexdump/hexdump.c Source/srec/srec.c \
Source/Common/utility.c

# Flatten object names
OBJS_RELEASE = $(addprefix $(RELEASE_DIR)/, $(notdir $(SRCS:.c=.o)))
OBJS_DEBUG   = $(addprefix $(DEBUG_DIR)/, $(notdir $(SRCS:.c=.o)))

vpath %.c Source Source/gzip Source/hexdump Source/srec Source/Common

# Executable name
TARGET = utility.exe

# Default rule
all: release

# Build release version
release: $(RELEASE_DIR)/$(TARGET)

$(RELEASE_DIR)/$(TARGET): $(OBJS_RELEASE)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(RELEASE_DIR)/%.o: %.c
	@mkdir -p $(RELEASE_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Build debug version
debug: $(DEBUG_DIR)/$(TARGET)

$(DEBUG_DIR)/$(TARGET): $(OBJS_DEBUG)
	$(CC) $(DEBUGFLAGS) -o $@ $^ $(LDFLAGS)

$(DEBUG_DIR)/%.o: %.c
	@mkdir -p $(DEBUG_DIR)
	$(CC) $(DEBUGFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(RELEASE_DIR) $(DEBUG_DIR)

.PHONY: all release debug clean

