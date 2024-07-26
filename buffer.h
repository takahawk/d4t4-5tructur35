#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

typedef struct {
	size_t len;
	void *data;
} Buffer;

// copies buffer
// memory can (and should be) freed using FreeBuffer 
static inline Buffer
B_Copy(Buffer buf) {
	Buffer newBuf;
	newBuf.len = buf.len;
	newBuf.data = malloc(newBuf.len);
	memcpy(newBuf.data, buf.data, buf.len);

	return newBuf;
}

// not copying the data, just wrap it inside buffer struct
// to have length attached 
// B_Free should be called for this kind of buffers ONLY if
// it is supposed to take of ownership of heap memory in data
static inline Buffer
B_Wrap(void* data, size_t len) {
	return (Buffer) {
		.data = data,
		.len = len
	};
}

// same as B_Wrap but make copy from data
static inline Buffer
B_From(void *data, size_t len) {
	return B_Copy(B_Wrap(data, len));
}


// compares two buffers byte-by-byte
// >0 if first is greater, 0 if equals, -1 if second if lesser
// length is considered if first bytes are the same: longer is greater
static inline int
B_Cmp(Buffer a, Buffer b) {
	int c = memcmp(a.data, b.data, MIN(a.len, b.len));
	if (c == 0) {
		// possible difference in length
		return a.len - b.len;
	}
	return c;
}


// creates null buffer. can be used as "no value" in function
// returns
static inline Buffer 
B_Null() {
       	return B_Wrap(NULL, 0);
}

// null check. returns true for all empty buffers, not only
// created by B_Null(). For freed buffers should also return true
static inline bool
B_IsNull(Buffer buf) {
	return buf.data == NULL && buf.len == 0;
}

static inline void
B_Nullify(Buffer *buf) {
	buf->data = NULL;
	buf->len = 0;
}

// just free memory and makes 'buf' a null buffer
static inline void
B_Free(Buffer* buf) {
	free(buf->data);
	B_Nullify(buf);
}
#endif
