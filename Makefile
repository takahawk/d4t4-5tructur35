CC=gcc
CFLAGS=-I.

.PHONY: all clean

all: array_list.o skip_list_map.o buffer.o

array_list.o: array_list.h array_list.c
	$(CC) -c -o $@ $< $(CFLAGS)

buffer.o: buffer.h buffer.c
	$(CC) -c -o $@ $< $(CFLAGS)

skip_list_map.o: skip_list_map.h skip_list_map.c
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o
