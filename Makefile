CC ?= gcc
CFLAGS=-I. $(if $(NODEBUG),,-g)
SRC=array_list.c skip_list_map.c
OBJ=$(SRC:.c=.o)
HEADERS=array_list.h buffer.h resizable_buffer.h skip_list_map.h string_.h

DYNAMIC_LIBRARY = libd4t4-5tructur35.so

INSTALL_INCLUDE_DIR ?= /usr/local/include/w/d4t4-5tructur35
INSTALL_SRC_DIR ?= /usr/local/src/w/d4t4-5tructur35
INSTALL_LIB_DIR ?= /usr/local/lib/

.DEFAULT_GOAL := $(DYNAMIC_LIBRARY)
.PHONY: check clean install

$(DYNAMIC_LIBRARY): $(OBJ)
	$(CC) -shared -o $@ $^


check: 
	$(CC) -c $(SRC)
	$(CC) -fsyntax-only $(HEADER_ONLY)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

install:
	mkdir -p $(INSTALL_INCLUDE_DIR) 
	mkdir -p $(INSTALL_SRC_DIR) 
	mkdir -p $(INSTALL_LIB_DIR)
	for header in $(HEADERS); do \
	       cp $$header $(INSTALL_INCLUDE_DIR) ; \
	done
	
	for src in $(SRC); do \
		cp $$src $(INSTALL_SRC_DIR) ; \
	done
	
	cp $(DYNAMIC_LIBRARY) $(INSTALL_LIB_DIR)
	


clean:
	rm -f *.o
	rm -f *.a
