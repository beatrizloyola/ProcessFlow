CC = gcc
CFLAGS = -Wall -Wextra -g
SRCS = main.c task.c exec.c job.c
TARGET = processflow

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

test: $(TARGET)
	@for d in Testes/*/; do \
		n=$$(basename $$d); \
		in=$$d$$n-entrada.txt; \
		exp=$$d$$n-saida.txt; \
		out=$$(./$(TARGET) < $$in); \
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
