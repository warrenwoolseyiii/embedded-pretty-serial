CC = g++
CFLAGS = -Wall -Wextra -pedantic -Isrc
LDFLAGS = -DSERIAL_EN=1 -DSYS_DEBUG_EN=1

SRCDIR = ../src
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)

TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.cpp $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)