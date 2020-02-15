CC = gcc
CFLAGS = -I. -g
TARGET = oss
OBJS = oss.o
.SUFFIXES: .c .o

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
