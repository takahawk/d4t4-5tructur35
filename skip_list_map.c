#include "skip_list_map.h"

#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

static inline int
_Cmp(Buffer a, Buffer b) {
	// TODO: move it to buffer probably?
	int c = memcmp(a.data, b.data, MIN(a.len, b.len));
	if (c == 0) {
		// possible difference in length
		return a.len - b.len;
	}
	return c;
}

static inline _SLM_Node*
_AllocNode(Buffer key, Buffer value) {
	_SLM_Node *node = malloc(sizeof(_SLM_Node));
	node->key = key;
	node->value = value;

	return node;
}

static _SLM_Node*
_GetNode(SkipListMap *slm, Buffer key) {
	_SLM_Node* node = slm->head;
outer:
	while (node) {
		int c = _Cmp(key, node->key);
		if (c < 0) {
			// lesser than any node - our node not in the list
			return NULL;
		} else if (c == 0) {
			return node;
		} else {
			while (node->next) {
				c = _Cmp(key, node->key);
				if (c >= 0) {
					node = node->next;
					goto outer;
				} else {
					node = node->lower;
				}
			}
		}
	}

	return NULL;
}

SkipListMap
SLM_Create() {
	return (SkipListMap) {
		.head = NULL
	};
}

void
SLM_Set(SkipListMap *slm, Buffer key, Buffer value) {
	// TODO: implement
}

Buffer
SLM_Get(SkipListMap *slm, Buffer key) {
	return _GetNode(slm, key)->value;
}

void
SLM_Delete(SkipListMap *slm, Buffer key) {
	// TODO: implement
}

void
SLM_Free(SkipListMap *slm) {
	// TODO: implement
}
