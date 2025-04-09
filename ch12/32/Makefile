CC = gcc
CFLAGS = -m64 -Wall

all: csapp.o tfgets-select.o main.c
	$(CC) $(CFLAGS) -o main $^
	
csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

tfgets-select.o: tfgets-select.c
	$(CC) $(CFLAGS) -c tfgets-select.c
	
.PHONY: clean
clean:
	rm -rf main
	rm -rf csapp.o