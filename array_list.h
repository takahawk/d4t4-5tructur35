#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_
typedef struct {
	int elemSize;
	int capacity;
	int len;
	void *data;
} ArrayList;

ArrayList*
AllocArrayList(int elemSize, int capacity);

void
AL_Add(ArrayList*, void*);

void*
AL_Get(ArrayList*, int);

// returns count of removed elements
int
AL_Remove(ArrayList*, void*);

void
AL_RemoveAt(ArrayList*, int);

void
AL_Swap(ArrayList*, int, int);

void
FreeArrayList(ArrayList*);

#endif
