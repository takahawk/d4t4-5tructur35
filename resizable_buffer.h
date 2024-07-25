#ifndef RESIZABLE_BUFFER_H_
#define RESIZABLE_BUFFER_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "buffer.h"

#define RESIZE_FACTOR 1.5
#define MIN_BUFFER_SIZE 10

typedef struct {
	void *data;
	size_t len;
	size_t capacity;
} ResizableBuffer;

// create resizable buffer with a given capacity
static inline ResizableBuffer
RB_Create(size_t capacity) {
	capacity = MIN(capacity, MIN_BUFFER_SIZE);
	ResizableBuffer rb = {
		.data = malloc(capacity),
		.len = 0,
		.capacity = capacity
	};

	return rb;
}

// returns resizable buffer that manages memory from original buffer
// original buffer is zeroed out by this function
// dont try to make out resizable buffer from buffer that is mapping
// memory not from heap pls
static inline ResizableBuffer
RB_AsResizable(Buffer *b) {
	ResizableBuffer rb = {
		.data = b->data,
		.len = b->len,
		.capacity = b->len
	};
	b->data = NULL;
	b->len = 0;
	return rb;
}

// shrinks unused memory and returns simple buffer.
// resizable buffer instance will be zeroed out
static inline Buffer
RB_ToBuffer(ResizableBuffer *rb) {
	Buffer b = {
		.data = realloc(rb->data, rb->len),
		.len = rb->len
	};
	rb->data = NULL;
	rb->len = 0;
	rb->capacity = 0;

	return b;
}

// copies data from another buffer appending data to resizable buffer
// reallocates memory automatically if needed
static inline void
RB_Append(ResizableBuffer *rb, Buffer b) {
	size_t len = rb->len + b.len;

	if (len > rb->capacity) {
		while (len > rb->capacity)
			rb->capacity *= RESIZE_FACTOR;
		rb->data = realloc(rb->data, rb->capacity);
	}

	memcpy((char *) rb->data + rb->len, b.data, b.len);
	rb->len = len;
}

static inline void
RB_Free(ResizableBuffer *rb) {
	free(rb->data);
	rb->data = NULL;
	rb->len = 0;
	rb->capacity = 0;
}

#endif
