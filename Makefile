SHELL := /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra -g
SRCDIR = source
SRCS = $(SRCDIR)/main.c $(SRCDIR)/task.c $(SRCDIR)/exec.c $(SRCDIR)/job.c
TARGET = processflow

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

test: $(TARGET)
	@for d in tests/*/; do \
		n=$$(basename $$d); \
		in=$$d$$n-entrada.txt; \
		exp=$$d$$n-saida.txt; \
		if [ "$$n" = "teste3" ]; then \
			out=$$(./$(TARGET) $$in); \
		else \
			out=$$(./$(TARGET) < $$in); \
		fi; \
		if [ "$$out" = "$$(cat $$exp)" ]; then \
			echo "$$n: PASSOU"; \
		else \
			echo "$$n: FALHOU"; \
			diff <(echo "$$out") $$exp; \
		fi; \
	done

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean test
