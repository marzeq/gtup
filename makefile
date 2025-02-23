CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic \
         -std=c17
DEVFLAGS = -O0 -g3
RELEASEFLAGS = -O2
BUILD_DIR = build
SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET = $(BUILD_DIR)/gtup

all: release

debug: CFLAGS += $(DEVFLAGS)
debug: $(TARGET)

release: CFLAGS += $(RELEASEFLAGS)
release: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: debug
	./$(TARGET) $(ARGS)

.PHONY: all debug release clean run
