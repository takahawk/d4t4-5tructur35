CC=gcc
CFLAGS=-I.
OBJ=array_list.o buffer.o skip_list_map.o

.PHONY: all clean

all: libd4t4-5tructur35.a

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

libd4t4-5tructur35.a: $(OBJ)
	ar rcs $@ $^

clean:
	rm -rf *.o
