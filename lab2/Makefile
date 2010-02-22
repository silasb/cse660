UNAME := $(SHELL uname)

CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -Wno-parentheses -pedantic -std=gnu99
LDFLAGS=-lpthread -lrt

SOURCES=src/main.c src/buffer.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bin/lab2

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.m.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*.o $(EXECUTABLE)
