CC = gcc
CFLAGS = -std=c11 -Wall

SRCS = http_server.c
OBJS = $(SRCS:.c=.o)
TARGET = bin/server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
