#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Buffer
AsBuffer(void* data, size_t len) {
	return (Buffer) {
		.data = data,
		.len = len
	};
}

Buffer
B_Copy(Buffer buf) {
	Buffer newBuf;
	newBuf.len = buf.len;
	newBuf.data = malloc(newBuf.len);
	memcpy(newBuf.data, buf.data, buf.len);

	return newBuf;
}

void
FreeBuffer(Buffer buf) {
	free(buf.data);
}
