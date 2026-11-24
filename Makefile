CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -Iinclude

SRC     = $(wildcard src/*.c)
OBJ     = $(patsubst src/%.c,build/%.o,$(SRC))
BUILD   = build

ifeq ($(OS),Windows_NT)
    MKDIR = if not exist $(BUILD) mkdir $(BUILD)
    RMDIR = if exist $(BUILD) rmdir /s /q $(BUILD)
else
    MKDIR = mkdir -p $(BUILD)
    RMDIR = rm -rf $(BUILD)
endif

.PHONY: all clean

all: $(OBJ)

build/%.o: src/%.c
	$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RMDIR)