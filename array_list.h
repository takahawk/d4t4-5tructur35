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
ArrayListAdd(ArrayList*, void*);

void*
ArrayListGet(ArrayList*, int);

// returns count of removed elements
int
ArrayListRemove(ArrayList*, void*);

void
ArrayListRemoveAt(ArrayList*, int);

void
FreeArrayList(ArrayList*);

#endif
