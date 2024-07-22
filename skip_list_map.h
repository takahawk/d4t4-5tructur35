#ifndef SKIP_LIST_MAP_H_
#define SKIP_LIST_MAP_H_

#include "array_list.h"
#include "buffer.h"

typedef struct _SLM_Node {
	ArrayList nexts;
	ArrayList prevs;
	Buffer key;
	Buffer value;
} _SLM_Node;

typedef struct {
	_SLM_Node *head;
} SkipListMap;

SkipListMap
SLM_Create();

void
SLM_Set(SkipListMap *slm, Buffer key, Buffer value);

Buffer
SLM_Get(SkipListMap slm, Buffer key);

void
SLM_Delete(SkipListMap *slm, Buffer key);

void
SLM_Free(SkipListMap *slm);

#endif
