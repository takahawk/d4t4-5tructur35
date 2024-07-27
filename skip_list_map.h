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

static inline SkipListMap
SLM_Create() {
	return (SkipListMap) {
		.head = NULL
	};
}

// note: key and value buffers are passed as-is, so if you modify them outside
//       they will be modified there, so don't do this for keys at least if
//       do not want to break a map
void
SLM_Iterate(SkipListMap slm, void (*iter)(Buffer, Buffer, void*), void *arg);

void
SLM_Set(SkipListMap *slm, Buffer key, Buffer value);

// returns NullBuffer on error
Buffer
SLM_Get(SkipListMap slm, Buffer key);

void
SLM_Delete(SkipListMap *slm, Buffer key);

void
SLM_Free(SkipListMap *slm);

#endif
