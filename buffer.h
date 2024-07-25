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

// not copying the data, just wrap it inside buffer struct
// to have length attached 
// B_Free shouldn't be called for this kind of buffers
static inline Buffer
B_As(void* data, size_t len) {
	return (Buffer) {
		.data = data,
		.len = len
	};
}

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
       	return B_As(NULL, 0);
}

// null check. returns true for all empty buffers, not only
// created by B_Null(). For freed buffers should also return true
static inline bool
B_IsNull(Buffer buf) {
	return buf.data == NULL && buf.len == 0;
}

// just free memory and makes 'buf' a null buffer
static inline void
B_Free(Buffer* buf) {
	free(buf->data);
	buf->data = NULL;
	buf->len = 0;
}
#endif
