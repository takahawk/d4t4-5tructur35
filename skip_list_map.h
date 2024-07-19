#ifndef SKIP_LIST_MAP_H_
#define SKIP_LIST_MAP_H_

#include "buffer.h"

typedef struct {
	Buffer key;
	Buffer value;
} _SLM_KeyValue;

typedef struct _SLM_Node {
	_SLM_Node *next;
	_SLM_Node *lower;
	_SLM_KeyValue *keyValue;
} _SLM_Node;

typedef struct {
	_SLM_Node *head;
} SkipListMap;

SkipListMap
SLM_Create();

void
SLM_Add(SkipListMap *slm, Buffer key, Buffer value);

Buffer
SLM_Get(SkipListMap *slm, Buffer key);

void
SLM_Delete(SkipListMap *slm, Buffer key);

void
SLM_Free(SkipListMap *slm);

#endif
