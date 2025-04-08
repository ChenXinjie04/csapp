CFLAGS = -O0 -m64 -Wall
CC = gcc

all: tiny

tiny: csapp.o main.c
	$(CC) $(CFLAGS) -o tiny $^

csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

.PHONY: clean
clean:
	rm -rf csapp.o
	rm -rf tiny
