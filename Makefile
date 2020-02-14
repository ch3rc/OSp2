CC = gcc
CFLAGS = -I. -g
TARGET = oss
OBJS = main.o flags.o
.SUFFIXES: .c .o

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
