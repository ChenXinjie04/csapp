CC = gcc
CFLAGS = -m64 -pthread

all: csapp.o main.c
	$(CC) $(CFLAGS) -o main $^
	
csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

.PHONY: clean
clean:
	rm -rf csapp.o
	rm -rf main
