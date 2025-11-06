CC := gcc

SRC_DIR := src

BUILD_DIR := build

CFLAGS := -std=c89 -Isrc -Wall -Wextra -MMD -MP -pedantic

LDFLAGS := -flto -Wl,--gc-sections

SRC := $(shell find -L $(SRC_DIR) -type f -name '*.c')

OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

DEP := $(OBJ:.o=.d)

BIN := main

all: $(BIN)
	@echo "Build complete."

$(BIN): $(OBJ)
	@$(CC) $(LDFLAGS) $(OBJ) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	
run: $(BIN)
	./$(BIN)

valgrind: $(BIN)
	valgrind --leak-check=yes ./$(BIN)
	
clean:
	@rm -rf $(BUILD_DIR) $(BIN)
	
print:
	@echo "Källfiler: $(SRC)"
	@echo "Objektfiler: $(OBJ)"
	@echo "Dependency-filer: $(DEP)"
	
-include $(DEP)

.PHONY: all run clean