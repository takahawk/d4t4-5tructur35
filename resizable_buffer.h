#ifndef RESIZABLE_BUFFER_H_
#define RESIZABLE_BUFFER_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "buffer.h"
#include "string_.h"

#define RESIZE_FACTOR 1.5
#define MIN_BUFFER_SIZE 10

// appends given chunk to resizable buffer. removed null terminator
// if it is present at the end of the buffer before append
// note: if zero bytes need to be preserved just design different function
// (something like RB_AppendW0
#define RB_Append(self, X, ...) _Generic((X), \
	Buffer: RB_AppendBuffer,   \
	String: RB_AppendString,   \
	char*: RB_AppendCstr,      \
	void*: RB_AppendVoid       \
	)(self, X __VA_OPT__(,) __VA_ARGS__)

typedef struct {
	size_t capacity;
	void *data;
	size_t len;
} ResizableBuffer;

// create resizable buffer with a given capacity
static inline ResizableBuffer
RB_Create(size_t capacity) {
	capacity = MAX(capacity, MIN_BUFFER_SIZE);
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
RB_FromBuffer(Buffer *b) {
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


// copiest data from void pointer appending given number of bytes to the
// end of resizable buffer
static inline void
RB_AppendVoid(ResizableBuffer *rb, void* v, size_t vlen) {
	size_t len = rb->len + vlen;

	if (len > rb->capacity) {
		while (len > rb->capacity)
			rb->capacity *= RESIZE_FACTOR;
		rb->data = realloc(rb->data, rb->capacity);
	}
	char *p = (char *) rb->data + rb->len;
	if (rb->len != 0)
		while (*p == '\0') {
			p--;
			len--;
		}

	memcpy((char *) rb->data + rb->len, v, vlen);
	rb->len = len;
}

// copies data from another buffer appending data to resizable buffer
// reallocates memory automatically if needed
static inline void
RB_AppendBuffer(ResizableBuffer *rb, Buffer b) {
	RB_AppendVoid(rb, b.data, b.len);
}

// appends string to resizable buffer. If buffer is null-terminated that
// null symbol will be removed to make data null-terminated after new
// appended bytes from string
static inline void
RB_AppendString(ResizableBuffer *rb, String s) {
	if (rb->len > 0 &&
	    s.len > 0 &&
	    ((char *) rb->data)[rb->len - 1] == '\0') {
		rb->len--;
	}

	RB_AppendVoid(rb, s.str, s.len);
}

// appends c string to resizable buffer. treats it in a similar way as
// RB_AppendString. c string is assumed to be null terminated
static inline void
RB_AppendCstr(ResizableBuffer *rb, char* cstr) {
	RB_AppendString(rb, S_Wrap(cstr));
}

static inline void
RB_Free(ResizableBuffer *rb) {
	free(rb->data);
	rb->data = NULL;
	rb->len = 0;
	rb->capacity = 0;
}

#endif
