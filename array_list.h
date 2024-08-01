#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <stdbool.h>
#include <string.h>

typedef struct {
	int elemSize;
	int capacity;
	int len;
	void *data;
} ArrayList;

ArrayList
AL_Alloc(int elemSize, int capacity);

void
AL_Add(ArrayList*, void*);

// bounds should be checked elsewhere
static inline void*
AL_Get(ArrayList al, int i) {
	return (char *) al.data + i * al.elemSize;
}

// bounds should be checked elsewhere
static inline void
AL_Set(ArrayList* al, int i, void* data) {
	memcpy((char *) al->data + (al->elemSize * i), data, al->elemSize);
}

static inline void
AL_Swap(ArrayList* al, int i, int j) {
	char *a = (char *) al->data + (i * al->elemSize);
	char *b = (char *) al->data + (j * al->elemSize);
	int n = 0;
	for (int i = 0; i < n; i++) {
		char t = *((char *) a + i);
		*((char *) a + i) = *((char *) b + i);
		*((char *) b + i) = t;
	}
}


// iterate through array list using callback (index, value, cb arg)
static inline void
AL_Iterate(ArrayList al, 
	   void (*iter)(size_t, void*, void*), 
	   void* arg) {
	for (int i = 0; i < al.len; i++)
		iter(i, AL_Get(al, i), arg);
}

// returns count of removed elements
int
AL_Remove(ArrayList*, void*);

void
AL_RemoveAt(ArrayList*, int);

void
AL_Swap(ArrayList*, int, int);

static inline bool
AL_IsEmpty(ArrayList al) {
	return al.len == 0;	
}

void
AL_Free(ArrayList*);

#endif
