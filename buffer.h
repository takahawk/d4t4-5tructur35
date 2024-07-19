#ifndef BUFFER_H_
#define BUFFER_H_

#include <stddef.h>

typedef struct {
	void *data;
	size_t len;
} Buffer;

// not copying the data, just wrap it inside buffer struct
// to have length attached and possibly manage memory by it
// in the future
Buffer
ToBuffer(void*, size_t);

// copy data to newly create buffer structure
Buffer
CopyBuffer(void*, size_t);

void
FreeBuffer(Buffer);
#endif
