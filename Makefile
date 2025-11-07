CC := gcc
CFLAGS := -std=c89 -Wall -Wextra -pedantic
BINARY := main

all: run

build:
	$(CC) $(CFLAGS) src/main.c -o $(BINARY) 

run: build
	./$(BINARY)

test:
	$(CC) $(CLFAGS) tests/test.c -o test
	./test
	rm test

valgrind: build
	valgrind --leak-check=yes ./$(BINARY)