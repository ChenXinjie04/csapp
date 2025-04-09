CC = gcc
CFLAGS = -m64 -Wall

all: csapp.o tfgets-thread.o main.c
	$(CC) $(CFLAGS) -o main $^
	
csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

tfgets-thread.o: tfgets-thread.c
	$(CC) $(CFLAGS) -c tfgets-thread.c
	
.PHONY: clean
clean:
	rm -rf main
	rm -rf *.o