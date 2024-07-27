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

// note: key and value buffers are returned as-is, so use copies if you need
//       to modify their value or be ready for unexpected consequences
static inline void
SLM_Iterate(SkipListMap slm, void (*iter)(Buffer, Buffer, void*), void *arg) {
	_SLM_Node *node = slm.head;
	
	while (node) {
		iter(node->key, node->value, arg);
		node = (_SLM_Node *) AL_Get(node->nexts, 0);
	}
}

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
