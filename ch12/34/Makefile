CC = gcc
CFLAGS = -m64 -O0

all: serial concurrent

serial: csapp.o serial.c
	$(CC) $(CFLAGS) -o serial $^

concurrent: csapp.o concurrent.c
	$(CC) $(CFLAGS) -o concurrent $^

csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf serial
	rm -rf concurrent
