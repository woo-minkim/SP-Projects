CC = gcc
# CFLAGS = -Wall -Wextra -g

SRCS = bitmap.c debug.c hash.c hex_dump.c list.c main.c
OBJS = $(SRCS:.c=.o)

TARGET = testlib

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
