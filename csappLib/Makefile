CC = gcc
CFLAGS = -m64 -Wall -Werror

run: all
	./main

all: src/main.c src/csapp.c src/csapp.h
	$(CC) $(CFLAGS) -O0 -o main src/main.c src/csapp.c

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf main
