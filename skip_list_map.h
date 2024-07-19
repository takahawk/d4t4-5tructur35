#ifndef SKIP_LIST_MAP_H_
#define SKIP_LIST_MAP_H_

#include "buffer.h"

typedef struct _SLM_Node {
	struct _SLM_Node *next;
	struct _SLM_Node *lower;
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
SLM_Get(SkipListMap *slm, Buffer key);

void
SLM_Delete(SkipListMap *slm, Buffer key);

void
SLM_Free(SkipListMap *slm);

#endif
