CC=gcc
CFLAGS=-I.

.PHONY: all clean

all: array_list.o skip_list_map.o buffer.o

array_list.o: array_list.c array_list.h
	$(CC) -c -o $@ $< $(CFLAGS)

buffer.o: buffer.c buffer.h
	$(CC) -c -o $@ $< $(CFLAGS)

skip_list_map.o: skip_list_map.c skip_list_map.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o
