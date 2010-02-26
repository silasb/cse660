UNAME := $(SHELL uname)

CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -Wno-parentheses -pedantic -std=gnu99
LDFLAGS=

EXECUTABLE=bin/lab3
SOURCES=src/main.c
OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.m.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*.o $(EXECUTABLE)
