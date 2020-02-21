CC = gcc
CFLAGS = -I. -g
.SUFFIXES: .c .o

all: oss handleChild

oss: oss.o
	$(CC) $(CFLAGS) -o $@ oss.o

handleChild: handleChild.o
	$(CC) $(CFLAGS) -o $@ handleChild.o 

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o oss handleChild
