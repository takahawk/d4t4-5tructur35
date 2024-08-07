#include "array_list.h"

#include <stdlib.h>
#include <string.h>

#include "w/ut1l5/math.h"

#define MINIMUM_CAPACITY 2

inline static void
_Clear(ArrayList*);

ArrayList
AL_Alloc(int elemSize, int capacity) {
	ArrayList al;
	al.elemSize = elemSize;
	al.capacity = u_max(MINIMUM_CAPACITY, capacity);
	al.len = 0;
       	al.data = malloc(elemSize * capacity);
	memset(al.data, 0, elemSize * capacity);
	return al;
}

void
AL_Add(ArrayList* al, void* elem) {
	if (al->len == al->capacity) {
		// mb make it adjustable?
		al->capacity *= 2;
		al->data = realloc(al->data, al->capacity * al->elemSize);
	}

	memcpy((char*) al->data + (al->len++ * al->elemSize), 
	       elem,
	       al->elemSize);
}


int
AL_Remove(ArrayList* al, void* elem) {
	int c = 0;
	for (int i = 0; i < al->len; i ++) {
		if (!memcmp((char *) al->data + i * al->elemSize, 
			    elem, 
			    al->elemSize)) {
			// room for optimization? first, find all the elements
			// then remove them all together in one take
			AL_RemoveAt(al, i);
			c++;
		}
	}

	return c;
}

void
AL_RemoveAt(ArrayList* al, int i) {
	if (i < --al->len) {
		// move everything down if not the last element
		char *ptr = (char *) al->data + i * al->elemSize;
		memmove(ptr, ptr + 1, i * al->elemSize);
	}

	memset((char *) al->data + al->len * al->elemSize, 
	       0,
	       al->elemSize);
}

void
AL_Free(ArrayList* al) {
	free(al->data);
	al->data = NULL;
	al->capacity = 0;
	al->len = 0;
}


inline static void
_Clear(ArrayList* al) {
	memset(al->data, 0, al->len * al->elemSize);
	al->len = 0;

}
