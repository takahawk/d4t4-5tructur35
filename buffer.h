#ifndef BUFFER_H_
#define BUFFER_H_

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

void
FreeBuffer(Buffer);
#endif
