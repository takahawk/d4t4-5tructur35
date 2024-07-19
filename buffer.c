#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Buffer
ToBuffer(void* data, size_t len) {
	return (Buffer) {
		.data = data,
		.len = len
	};
}

Buffer
CopyBuffer(void* data, size_t len) {
	Buffer buf;
	buf.len = len;
	memcpy(buf.data, data, len);
	return buf;
}


void
FreeBuffer(Buffer buf) {
	free(buf.data);
}
