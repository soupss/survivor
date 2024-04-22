CC := clang
SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj
BIN_DIR := bin
EXECUTABLE := $(BIN_DIR)/out

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
CFLAGS := -I$(INC_DIR) $(shell pkg-config --cflags raylib) -Wall -Wextra
LDFLAGS := $(shell pkg-config --libs raylib)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -f $(EXECUTABLE) $(OBJ_DIR)/*
