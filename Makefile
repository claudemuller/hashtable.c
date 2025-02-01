CC := clang
DBG_BIN := lldb
CFLAGS += -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror
CFLAGS += -Wmissing-declarations
ASANFLAGS=-fsanitize=address -fno-common -fno-omit-frame-pointer
CFLAGS += 
LDFLAGS := 
LIBS := 
SRC_FILES := ./src/main.c
BIN_DIR := ./bin
BIN := $(BIN_DIR)/ht

ifeq ($(shell uname -s), Linux)
	CFLAGS = -D_GNU_SOURCE
else
	BIN = $(BIN_DIR)/ht.exe
endif

build: bin-dir
	$(CC) $(CFLAGS) $(LIBS) $(ASANFLAGS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

bin-dir:
	@mkdir -p $(BIN_DIR)

debug: debug-build
	$(DBG_BIN) $(BIN) $(ARGS)

debug-build: bin-dir
	$(CC) $(CFLAGS) -g $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

run: build
	@$(BIN) $(ARGS)

