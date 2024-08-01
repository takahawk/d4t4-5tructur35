#include "skip_list_map.h"

#include <stdlib.h>
#include <string.h>

#define AL_START_CAPACITY 2 


static inline int
_TossCoin() {
	return rand() > (RAND_MAX / 2.);
}

static inline void
_AL_AddNode(ArrayList *al, _SLM_Node *node) {
	AL_Add(al, &node);
}

static inline void
_AL_SetNode(ArrayList *al, int i, _SLM_Node *node) {
	AL_Set(al, i, &node);
}

static inline _SLM_Node*
_AL_GetNode(ArrayList al, int i) {
	return *((_SLM_Node**) AL_Get(al, i));
}

static inline void
_Expand(_SLM_Node *node) {
	_AL_AddNode(&node->nexts, NULL);
	_AL_AddNode(&node->prevs, NULL);
}

// alloc node. with levels. 
// returned value is a pointer to lowest (most common) level
static _SLM_Node*
_AllocNode(Buffer key, Buffer value) {
	_SLM_Node *node = malloc(sizeof(_SLM_Node));
	node->nexts = AL_Alloc(sizeof(_SLM_Node*), AL_START_CAPACITY);
	node->prevs = AL_Alloc(sizeof(_SLM_Node*), AL_START_CAPACITY);
	node->key = B_Copy(key);
	node->value = B_Copy(value);

	while (_TossCoin()) {
		_Expand(node);
	}

	return node;
}

static void
_FreeNode(_SLM_Node *node) {
	AL_Free(&node->nexts);
	AL_Free(&node->prevs);
	B_Free(&node->key);
	B_Free(&node->value);
}

static _SLM_Node*
_GetNode(SkipListMap slm, Buffer key) {
	_SLM_Node *node = slm.head;
	int c = B_Cmp(key, node->key);
	if (c < 0)
		return NULL;
	if (c == 0)
		return node;
	int level = node->nexts.len;
outer:
	while (node && level >= 0) {
		_SLM_Node *next = _AL_GetNode(node->nexts, level);
		c = B_Cmp(key, next->key);

		if (c > 0) {
			node = next;
			goto outer;
		}

		if (c == 0)
			return next;
	
		level--;
	}

	return NULL;
}

static void
_SetNodeValue(_SLM_Node *node, Buffer newValue) {
	Buffer value = node->value;

	value.len = newValue.len;
	value.data = realloc(value.data, value.len);
	memcpy(value.data, newValue.data, value.len);
}

static _SLM_Node*
_GetRightWithLevel(_SLM_Node *node, int level) {
	while (node && node->nexts.len <= level)
		node = _AL_GetNode(node->nexts, level);
	return node;
}

static _SLM_Node*
_GetLeftWithLevel(_SLM_Node *node, int level) {
	while (node && node->prevs.len <= level)
		node = _AL_GetNode(node->prevs, level);

	return node;
}

static void
_ConnectLeft(_SLM_Node *left, _SLM_Node *right, int expandLeft) {
	int level = 0;
	do {
		_AL_SetNode(&left->nexts, level, right);
		if (right)
			_AL_SetNode(&right->prevs, level, left);

		level++;
		right = _GetRightWithLevel(right, level);
		if (left->nexts.len == level && (right && expandLeft))
			_Expand(left);
	} while (left->nexts.len > level);
}

static void
_ConnectRight(_SLM_Node *left, _SLM_Node *right) {
	int level = 0;
	do {
		if (left)
			_AL_SetNode(&left->nexts, level, right);
		_AL_SetNode(&right->prevs, level, left);

		level++;
		left = _GetLeftWithLevel(left, level);
	} while (right->nexts.len > level);
}

void
SLM_Set(SkipListMap *slm, Buffer key, Buffer value) {
	if (slm->head == NULL) {
		slm->head = _AllocNode(key, value);
		return;
	}

	int c = B_Cmp(key, slm->head->key);

	if (c < 0) {
		_SLM_Node *node = _AllocNode(key, value);
		_ConnectLeft(node, slm->head, 1);
		slm->head = node;
		return;
	} else if (c == 0) {
		_SetNodeValue(slm->head, value);
		return;
	}	

	_SLM_Node *node = slm->head;
	int level = node->nexts.len - 1;

	do {
		_SLM_Node *next = _AL_GetNode(node->nexts, level);
		if (!next) {
			level--;
			continue;
		}
		c = B_Cmp(key, next->key);
		if (c < 0) {
			if (level == 0) {
				_SLM_Node *newNode = _AllocNode(key, value);
				_ConnectRight(node, newNode);
				_ConnectLeft(newNode, next, 0);
				return;
			}
			level--;
			continue;
		} else if (c == 0) {
			_SetNodeValue(next, value);
			return;
		}

		node = next;
	} while (level >= 0);
	
	_SLM_Node *newNode = _AllocNode(key, value);
	_ConnectRight(node, newNode);

	for (int i = 0; i < newNode->nexts.len; i++) {
		_AL_SetNode(&newNode->nexts, i, NULL);
	}
}


Buffer
SLM_Get(SkipListMap slm, Buffer key) {
	_SLM_Node *node = _GetNode(slm, key);
	if (node)
		return node->value;

	return B_Null();
}

void
SLM_Delete(SkipListMap *slm, Buffer key) {
	_SLM_Node *node = _GetNode(*slm, key);
	if (!node)
		return;
	if (node == slm->head) {
		// case 1: deleted node is first (head)
		slm->head = _AL_GetNode(node->nexts, 0);

		if (slm->head != NULL) {
			_SLM_Node *head = slm->head;
			// TODO: probably memset whole arraylist data
			//       to NULL would be faster?
			for (int i = 0; i < head->prevs.len; i++) {
				_AL_SetNode(&head->prevs, i, NULL);
			}	
			_SLM_Node *newNext = _AL_GetNode(head->nexts, 0);
			if (newNext) {
				// promote new head to highest level
				_ConnectLeft(head, newNext, 1);
			} else {
				// the new head is the only node in a list now
				for (int i = 0; i < head->nexts.len; i++) {
					_AL_SetNode(&head->nexts, i, NULL);
				}

			}
		}
		goto cleanup;
	}

	if (_AL_GetNode(node->nexts, 0) == NULL) {
		// case 2: deleted node is last
		for (int i = 0; i < node->prevs.len; i++) {
			_SLM_Node *prev = _AL_GetNode(node->prevs, i);
			_AL_SetNode(&prev->nexts, i, NULL);
		}
		goto cleanup;
	}

	// case 3: node is somewhere in-between first and last
	for (int i = 0; i < node->prevs.len; i++) {
		_SLM_Node *prev = _AL_GetNode(node->prevs, i);
		_SLM_Node *next = _AL_GetNode(node->nexts, i);
		_AL_SetNode(&prev->nexts, i, next);
		_AL_SetNode(&next->prevs, i, prev);
	}

cleanup:
	_FreeNode(node);		
}

void
SLM_Iterate(SkipListMap slm, void (*iter)(Buffer, Buffer, void*), void *arg) {
	_SLM_Node *node = slm.head;
	
	while (node) {
		iter(node->key, node->value, arg);
		node = _AL_GetNode(node->nexts, 0);
	}
}

void
SLM_Free(SkipListMap *slm) {
	if (slm->head == NULL)
		return;

	_SLM_Node *node = slm->head;

	while (node) {
		_SLM_Node *next = _AL_GetNode(node->nexts, 0);
		_FreeNode(node);
		node = next;
	}
	
	slm->head = NULL;
}
