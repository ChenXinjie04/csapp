CC = gcc
CFLAGS = -m64 -pthread -DNDEBUG

all: csapp.o psum-mutex.c
	$(CC) $(CFLAGS) -o main $^

csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

clean:
	rm -rf csapp.o
	rm -rf main