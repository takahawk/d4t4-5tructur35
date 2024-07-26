CC=gcc
CFLAGS=-I. $(if $(NODEBUG),,-g)
SRC=array_list.c skip_list_map.c
OBJ=$(SRC:.c=.o)
HEADER_ONLY=buffer.h resizable_buffer.h string_.h

.PHONY: check clean


check: 
	gcc -c $(SRC)
	gcc -fsyntax-only $(HEADER_ONLY)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

libd4t4-5tructur35.a: $(OBJ)
	ar rcs $@ $^

clean:
	rm -f *.o
	rm -f *.a
