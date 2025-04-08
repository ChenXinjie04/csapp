CFLAGS = -Wall -m64 -O0
CC = gcc

server: csapp.o echoserveri.c
	$(CC) $(CFLAGS) -o echos $^

client: csapp.o echoclient.c
	$(CC) $(CFLAGS) -o echoc $^

csapp.o: csapp.h csapp.c
	cp ../../csappLib/src/csapp.c .
	cp ../../csappLib/src/csapp.h .
	$(CC) $(CFLAGS) -c csapp.c

clean:
	rm -rf csapp.o
	rm -rf echoc
	rm -rf echos
