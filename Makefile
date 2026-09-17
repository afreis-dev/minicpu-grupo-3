CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic

ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
.SHELLFLAGS := /C
TARGET = minicpu.exe
RUN = .\$(TARGET)
else
TARGET = minicpu
RUN = ./$(TARGET)
endif

.PHONY: all run clean

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

run: $(TARGET)
	$(RUN)

clean:
ifeq ($(OS),Windows_NT)
	if exist $(TARGET) del /Q $(TARGET)
else
	rm -f $(TARGET)
endif
