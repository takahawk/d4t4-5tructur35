#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <stdbool.h>

typedef struct {
	int elemSize;
	int capacity;
	int len;
	void *data;
} ArrayList;

ArrayList
AllocArrayList(int elemSize, int capacity);

void
AL_Add(ArrayList*, void*);

void*
AL_Get(ArrayList, int);

void
AL_Set(ArrayList*, int, void*);

static inline void
AL_Iterate(ArrayList al, void (*iter)(void*, ...)) {
	for (int i = 0; i < al.len; i++)
		iter(AL_Get(al, i));
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
FreeArrayList(ArrayList*);

#endif
