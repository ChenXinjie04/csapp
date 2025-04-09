CC = gcc
CFLAGS = -m64 -Wall

all: csapp.o tfgets-proc.o main.c
	$(CC) $(CFLAGS) -o main $^
	
csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

tfgets-proc.o: tfgets-proc.c
	$(CC) $(CFLAGS) -c tfgets-proc.c
	
.PHONY: clean
clean:
	rm -rf main
	rm -rf csapp.o