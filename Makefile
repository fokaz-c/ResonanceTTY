# Compiler and flags
CC       := gcc
CFLAGS   := -Wall -Wextra -g -Icore/include
LDFLAGS  :=
TARGET   := build/renderer

# Directories
SRC_DIR   := src
CORE_DIR  := core
BUILD_DIR := build

# Files
SRC_FILES  := $(wildcard $(SRC_DIR)/*.c)
CORE_FILES := $(wildcard $(CORE_DIR)/*.c)
OBJ_FILES  := $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRC_FILES) $(CORE_FILES)))

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ_FILES)
	@echo "Linking $@"
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compilation rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(CORE_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean rule
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean
