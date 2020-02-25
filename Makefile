CC = gcc
CFLAGS = -I. -Wall -g
.SUFFIXES: .c .o

all: oss handleChild

oss: oss.o
	$(CC) $(CFLAGS) -o $@ oss.o -pthread

handleChild: handleChild.o
	$(CC) $(CFLAGS) -o $@ handleChild.o -pthread

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o oss handleChild
