#include "skip_list_map.h"

#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#define AL_START_CAPACITY 2 

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
_AL_GetNode(ArrayList *al, int i) {
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
	node->nexts = AllocArrayList(sizeof(_SLM_Node*), AL_START_CAPACITY);
	node->prevs = AllocArrayList(sizeof(_SLM_Node*), AL_START_CAPACITY);
	node->key = B_Copy(key);
	node->value = B_Copy(value);

	while (_TossCoin()) {
		_Expand(node);
	}

	return node;
}

static void
_FreeNode(_SLM_Node *node) {
	FreeArrayList(&node->nexts);
	FreeArrayList(&node->prevs);
	FreeBuffer(&node->key);
	FreeBuffer(&node->value);
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
		node = _AL_GetNode(&node->nexts, level);
	return node;
}

static _SLM_Node*
_GetLeftWithLevel(_SLM_Node *node, int level) {
	while (node && node->prevs.len <= level)
		node = _AL_GetNode(&node->prevs, level);

	return node;
}

static void
_ConnectLeft(_SLM_Node *left, _SLM_Node *right, int expandLeft) {
	int level = 0;
	do {
		_AL_SetNode(&left->nexts, level, right);
		_AL_SetNode(&right->prevs, level, left);

		level++;
		right = _GetRightWithLevel(right, level);
		if (left->nexts.len == level && (right && expandLeft))
			_Expand(left);
	} while (left->nexts.len < level);
}

static void
_ConnectRight(_SLM_Node *left, _SLM_Node *right) {
	int level = 0;
	do {
		_AL_SetNode(&left->nexts, level, right);
		_AL_SetNode(&right->prevs, level, left);

		level++;
		left = _GetLeftWithLevel(left, level);
	} while (right->nexts.len < level);
}

SkipListMap
SLM_Create() {
	return (SkipListMap) {
		.head = NULL
	};
}

void
SLM_Set(SkipListMap *slm, Buffer key, Buffer value) {
	if (slm->head == NULL) {
		slm->head = _AllocNode(key, value);
		return;
	}

	int c = _Cmp(key, slm->head->key);

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
	// TODO: rewrite with using new array list layout

}

Buffer
SLM_Get(SkipListMap *slm, Buffer key) {
	// TODO: impelement
}

void
SLM_Delete(SkipListMap *slm, Buffer key) {
	// TODO: implement
}

void
SLM_Free(SkipListMap *slm) {
	if (slm->head == NULL)
		return;

	_SLM_Node *node = slm->head;

	while (node) {
		next = _AL_GetNode(node->nexts, 0);
		_FreeNode(node);
		node = next;
	}
	
	slm->head = NULL;
}
