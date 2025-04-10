CFLAGS = -O0 -m64 -Wall -pthread -g
CC = gcc

all: tiny adder

adder: csapp.o adder.c
	$(CC) $(CFLAGS) -o adder $^
	mv adder cgi-bin/

tiny: csapp.o main.c
	$(CC) $(CFLAGS) -o tiny $^

csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

.PHONY: clean
clean:
	rm -rf csapp.o
	rm -rf tiny
	rm -rf cgi-bin/adder
