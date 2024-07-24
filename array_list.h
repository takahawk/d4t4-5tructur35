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

void
AL_Set(ArrayList*, int, void*);

void
AL_Iterate(ArrayList, void (*iter)(void*));

void*
AL_Get(ArrayList, int);

// returns count of removed elements
int
AL_Remove(ArrayList*, void*);

void
AL_RemoveAt(ArrayList*, int);

void
AL_Swap(ArrayList*, int, int);

inline bool
AL_IsEmpty(ArrayList al) {
	return al.len == 0;	
}

void
FreeArrayList(ArrayList*);

#endif
