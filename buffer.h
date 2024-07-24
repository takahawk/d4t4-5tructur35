#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	void *data;
	size_t len;
} Buffer;

// not copying the data, just wrap it inside buffer struct
// to have length attached 
// FreeBuffer shouldn't be called for this kind of buffers
Buffer
AsBuffer(void*, size_t);

// copies buffer
// memory can (and should be) freed using FreeBuffer 
Buffer
B_Copy(Buffer);

static inline Buffer
NullBuffer() {
	return AsBuffer(NULL, 0);
}

static inline bool
IsNullBuffer(Buffer buf) {
	return buf.data == NULL && buf.len == 0;
}

void
FreeBuffer(Buffer*);
#endif
