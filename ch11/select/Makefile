CFLAGS = -Wall -m64
CC = gcc

all: csapp.o select.c
	$(CC) $(CFLAGS) -o echos $^
	
csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

.PHONY:clean
clean:
	rm -rf csapp.o
	rm -rf echos
