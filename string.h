#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>
#include <string.h>

#include "buffer.h"

// in essense a variant of buffer. functions from this unit are supposed
// to ensure null-termination and that it is preserved 
typedef struct {
	size_t len;
	char* str;
} String;

// basically just wraps C string. memory is presumed to be handled elsewhere
// without triggering S_Free. It is assumed that given string is already null
// terminated. In other case you can use combination of B_Copy and S_FromBuffer
static inline String
S_As(char* str) {
	return (String) {
		.len = strlen(str),
		.str = str
	};
}

// transforms buffer to string enforcing null termination by adding \0 (only 
// if needed). Nullifies given buffer link. Is is assumed that it is true
// buffer that was allocated on heap
static inline String
S_FromBuffer(Buffer *buf) {
	String s = {
		.len = buf->len,
		.str = (char*) buf->data
	};
	buf->data = NULL;
	buf->len = 0;
	if (s.str[buf->len - 1] != '\0') {
		s.str = realloc(s.str, ++s.len);
		s.str[s.len - 1] = '\0';
	}


	return s;
}

// transforms string to byte. Null-terminator will be preserved.
// Assumed that true (not created by S_As() string is used) but it will
// not panic at once (and that will also panic only if not null terminated is buffer
// passed!) as with S_FromBuffer if you violate memory segments
// so be careful. Original string will be nullified
static inline Buffer
S_ToBuffer(String *s) {
	Buffer b = {
		.len = s->len,
		.data = (void*) s->str,
	};

	s->len = 0;
	s->str = NULL;

	return b;
}

// free string memory and nullify it. just a reminder: don't try to free S_As 
// strings
static inline void
S_Free(String *s) {
	free(s->str);
	s->len = 0;
	s->str = NULL;
}

#endif
