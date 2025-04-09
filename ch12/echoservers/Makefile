CC = gcc
CFLAGS = -m64 -Wall

all: csapp.o main.c
	$(CC) $(CFLAGS) -o main $^
	
csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c
	
.PHONY: clean
clean:
	rm -rf main
	rm -rf csapp.o