CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = main.c task.c exec.c
TARGET = processflow

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
