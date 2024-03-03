all: cish

VERSION='"0.0.0"'
TARGET='"$(shell $(CC) -dumpmachine)"'

CFLAGS=-Wall -Wextra -pedantic -std=c99
LIBS:=-ldl

cish: main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

main.o: main.c
	$(CC) -DVERSION=$(VERSION) -DTARGET=$(TARGET) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<
