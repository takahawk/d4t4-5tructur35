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

static inline int
_TossCoin() {
	return rand() > (RAND_MAX / 2.);
}

// alloc node. with levels. 
// returned value is a pointer to lowest (most common) level
static _SLM_Node*
_AllocNode(Buffer key, Buffer value) {
	_SLM_Node *node = malloc(sizeof(_SLM_Node));
	node->next = NULL;
	node->prev = NULL;
	node->key = key;
	node->value = value;
	node->upper = NULL;

	while (_TossCoin()) {
		_SLM_Node *lowerNode = malloc(sizeof(_SLM_Node));
		lowerNode->next = NULL;
		lowerNode->prev = NULL;
		lowerNode->upper = node;
		node->lower = lowerNode;
		node = lowerNode;
	}
	node->lower = NULL;

	return node;
}

static _SLM_Node*
_ExpandUpper(_SLM_Node *node) {
	_SLM_Node *newNode = malloc(sizeof(_SLM_Node));
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->upper = NULL;
	newNode->lower = node;
	node->upper = newNode;
	return newNode;
}

static _SLM_Node*
_GetLowest(_SLM_Node *node) {
	while (node->lower)
		node = node->lower;

	return node;
}

static _SLM_Node*
_GetUpperLeft(_SLM_Node *node) {
	while (node && !node->upper) {
		node = node->prev;
	}

	return node;
}

static _SLM_Node*
_GetUpperRight(_SLM_Node *node) {
	while (node && !node->upper) {
		node = node->next;
	}

	return node;
}

// assumes lowest nodes are given
// fixing left node we connect it to nodes on the right
static void
_ConnectLeft(_SLM_Node* left, _SLM_Node* right, int expandLevel) {
	do {
		left->next = right;
		right->prev = left;
		right = _GetUpperRight(right);
		if (!left->upper && (expandLevel && right)) {
			left = _ExpandUpper(left);
		} else {
			left = left->upper;
		}
	} while(left);
}

// assumes lowest nodes are given
// fixing right node we connect it to nodes on the left
// we do not need expand level for this one
static void
_ConnectRight(_SLM_Node* left, _SLM_Node* right) {
	do {
		left->next = right;
		right->prev = left;
		left = _GetUpperLeft(left);
		right = right->upper;
	} while(right);
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

void
_SetNodeValue(_SLM_Node *node, Buffer newValue) {
	Buffer value = node->value;

	value.len = newValue.len;
	value.data = realloc(value.data, value.len);
	memcpy(value.data, newValue.data, value.len);
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
		_ConnectLeft(node, _GetLowest(slm->head), 1);
		slm->head = node;
		return;
	} else if (c == 0) {
		_SetNodeValue(slm->head, value);
		return;
	}	

	_SLM_Node *node = slm->head;
	while (node->next) {
		int c = _Cmp(key, node->next->key);
		if (c < 0) {
			if (node->lower) {
				node = node->lower;
				continue;
			}
			_SLM_Node *newNode = _AllocNode(key, value);
			_SLM_Node *next = node->next;
			_ConnectRight(node, newNode);
			_ConnectLeft(newNode, next, 0);
			return;
		} else if (c == 0) {
			_SetNodeValue(node->next, value);
			return;
		}

		node = node->next;
	}

	while(node->lower)
		node = node->lower;
	_SLM_Node *newNode = _AllocNode(key, value);
	_ConnectRight(node, newNode);

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
