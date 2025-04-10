CFLAGS = -O0 -m64 -Wall -pthread -Ilibs
CC = gcc

all: tiny adder

adder: csapp.o adder.c
	$(CC) $(CFLAGS) -o adder $^
	mv adder cgi-bin/

tiny: csapp.o threadPool.o sbuf.o main.c
	$(CC) $(CFLAGS) -o tiny $^

csapp.o: libs/csapp.h libs/csapp.c
	$(CC) $(CFLAGS) -c libs/csapp.c

threadPool.o: libs/threadPool.h libs/threadPool.c
	$(CC) $(CFLAGS) -c libs/threadPool.c

sbuf.o: libs/sbuf.h libs/sbuf.c
	$(CC) $(CFLAGS) -c libs/sbuf.c

.PHONY: clean
clean:
	rm -rf csapp.o
	rm -rf tiny
	rm -rf cgi-bin/adder
