CC=gcc
CFLAGS=-Wall -g
INCLUDE=-Ilibs
TARGET=main
SRCS=main.c libs/*.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o libs/*.o

run: $(TARGET)
	./$(TARGET)
