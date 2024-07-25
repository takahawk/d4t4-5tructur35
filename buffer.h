#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	void *data;
	size_t len;
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

// convenience function to easily represent c-strings as buffer
// not bothering to calculate it's length beforehand
// working similar to B_As so that it is just a wrapper
static inline Buffer
B_StringAs(char* str) {
	size_t len = strlen(str);
	return B_As(str, len);
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
