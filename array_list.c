#include "array_list.h"

#include <stdlib.h>
#include <string.h>

inline static void
_Clear(ArrayList*);

ArrayList*
AllocArrayList(int elemSize, int capacity) {
	ArrayList *al = malloc(sizeof(ArrayList));
	al->elemSize = elemSize;
	al->capacity = capacity;
	al->len = 0;
       	al->data = malloc(elemSize * capacity);
	memset(al->data, 0, elemSize * capacity);
}

void
ArrayListAdd(ArrayList*, void*) {
}

void*
ArrayListRemove(ArrayList*, void*) {
}

void
ArrayListRemoveAt(ArrayList*, int) {
}

void
FreeArrayList(ArrayList* al) {
	free(al->data);
	free(al);
}


inline static void
_Clear(ArrayList* al) {
	memset(al->data, 0, al->len * al->elemSize);
	al->len = 0;

}
