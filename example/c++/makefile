CC = g++
CFLAGS = -Wall -Wextra -pedantic -Isrc
LDFLAGS = -DSERIAL_EN=1 -DSYS_DEBUG_EN=1

SRCDIR = ../../src
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:.c=.o)

EMB_RBDIR = ../../deps/embedded-ring-buffer/src
EMB_RB_SOURCES = $(wildcard $(EMB_RBDIR)/*.c)
EMB_RB_OBJECTS = $(EMB_RB_SOURCES:.c=.o)

TARGET = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.cpp $(OBJECTS) $(EMB_RB_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EMB_RBDIR)/%.o: $(EMB_RBDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)