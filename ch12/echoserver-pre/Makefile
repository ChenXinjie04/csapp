CC = gcc
CFLAGS = -m64

echos: csapp.o sbuf.o echo-cnt.o echoserver-pre.c
	$(CC) $(CFLAGS) -o echos $^
	
csapp.o: csapp.h csapp.c
	$(CC) $(CFLAGS) -c csapp.c

sbuf.o: sbuf.h sbuf.c
	$(CC) $(CFLAGS) -c sbuf.c

echo-cnt.o: echo-cnt.c
	$(CC) $(CFLAGS) -c echo-cnt.c

.PHONY: clean
clean:
	rm -rf csapp.o
	rm -rf echos
