CC ?= gcc
CFLAGS=-I. $(if $(NODEBUG),,-g)
SRC=array_list.c string_.c skip_list_map.c
OBJ=$(SRC:.c=.o)
OBJ_PIC = $(OBJ:.o=_pic.o)
HEADERS=array_list.h buffer.h resizable_buffer.h skip_list_map.h string_.h

STATIC_LIBRARY = libd4t4-5tructur35.a
SHARED_LIBRARY = libd4t4-5tructur35.so

INSTALL_INCLUDE_DIR ?= /usr/local/include/w/d4t4-5tructur35
INSTALL_SRC_DIR ?= /usr/local/src/w/d4t4-5tructur35
INSTALL_LIB_DIR ?= /usr/local/lib/

.DEFAULT_GOAL := all
.PHONY: all check clean install shared static 

all: shared static

shared: $(SHARED_LIBRARY)

static: $(STATIC_LIBRARY)

$(STATIC_LIBRARY): $(OBJ)
	ar rcs $@ $^

$(SHARED_LIBRARY): $(OBJ_PIC)
	$(CC) -shared -o $@ $^


check: 
	$(CC) -fsyntax-only $(SRC) $(HEADERS)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

%_pic.o: %.c
	$(CC) -fPIC -c -o $@ $< $(CFLAGS)

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
	
	cp $(SHARED_LIBRARY) $(INSTALL_LIB_DIR)
	cp $(STATIC_LIBRARY) $(INSTALL_LIB_DIR)
	

clean:
	rm -f *.o *.a *.so
