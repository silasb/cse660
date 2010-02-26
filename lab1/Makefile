UNAME := $(SHELL uname)

CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -Wno-parentheses -pedantic -std=gnu99
LDFLAGS=

EXECUTABLE=bin/lab1
SOURCES=src/shell.c
OBJECTS=$(SOURCES:.c=.o)

all: create-bin $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.m.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*.o $(EXECUTABLE)

create-bin:
	mkdir -p bin

.PHONY: create-bin all
