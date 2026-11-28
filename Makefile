CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -Iinclude

LIB_SRC = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
BUILD   = build

ifeq ($(OS),Windows_NT)
    MKDIR  = if not exist $(BUILD) mkdir $(BUILD)
    RMDIR  = if exist $(BUILD) rmdir /s /q $(BUILD)
    RUN_RB = $(BUILD)\test_ring_buffer.exe
    RUN_LR = $(BUILD)\test_line_reader.exe
    RUN_TK = $(BUILD)\test_tokenizer.exe
    RUN_CLI = $(BUILD)\test_cli.exe
else
    MKDIR  = mkdir -p $(BUILD)
    RMDIR  = rm -rf $(BUILD)
    RUN_RB = ./$(BUILD)/test_ring_buffer.exe
    RUN_LR = ./$(BUILD)/test_line_reader.exe
    RUN_TK = ./$(BUILD)/test_tokenizer.exe
    RUN_CLI = ./$(BUILD)/test_cli.exe
endif

.PHONY: all test clean

all: $(BUILD)/test_ring_buffer.exe $(BUILD)/test_line_reader.exe $(BUILD)/test_tokenizer.exe $(BUILD)/test_cli.exe

$(BUILD)/%.exe: tests/%.c $(LIB_SRC) $(HEADERS)
	$(MKDIR)
	$(CC) $(CFLAGS) $(LIB_SRC) $< -o $@

test: all
	$(RUN_RB)
	$(RUN_LR)
	$(RUN_TK)
	$(RUN_CLI)
clean:
	$(RMDIR)