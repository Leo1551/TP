.PHONY: all clean run

CC=gcc
CFLAGS=-Wall -g
INCLUDE=-Ilibs
TARGET=main
SRCS=main.c libs/*.c


all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o libs/*.o

run: $(TARGET)
	./$(TARGET)
