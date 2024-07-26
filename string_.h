#ifndef STRING_H_
#define STRING_H_

#include <stddef.h>
#include <string.h>

#include "buffer.h"

#define S_From(X, ...) _Generic((X), \
	Buffer: S_FromBuffer,   \
	Buffer*: S_FromBufferP  \
	) (X __VA_OPT__(,) __VA_ARGS__)


// in essense a variant of buffer. functions from this unit are supposed
// to ensure null-termination and that it is preserved 
typedef struct {
	size_t len;
	char* str;
} String;

// basically just wraps C string. memory is presumed to be handled elsewhere
// without triggering S_Free. It is assumed that given string is already null
// terminated. In other case use S_From()
static inline String
S_Wrap(char* str) {
	return (String) {
		.len = strlen(str),
		.str = str
	};
}

static inline String
S_Null() {
	return (String) {
		.str = NULL,
		.len = 0
	};
}

static inline bool
S_IsNull(String s) {
	return s.str == NULL && s.len == 0;
}

static inline void
S_Nullify(String *s) {
	s->str = NULL;
	s->len = 0;
}

// transforms buffer to string enforcing null termination by adding \0 (only 
// if needed).  it is assumed that it is
// buffer that was allocated on heap
static inline String
S_FromBuffer(Buffer buf) {
	String s = {
		.len = buf.len,
		.str = (char*) buf.data
	};
	if (s.str[buf.len - 1] != '\0') {
		s.str = realloc(s.str, ++s.len);
		s.str[s.len - 1] = '\0';
	}


	return s;
}

// same as S_FromBuffer but nullifies given buffer link.
static inline String
S_FromBufferP(Buffer *buf) {
	String s = S_FromBuffer(*buf);
	buf->data = NULL;
	buf->len = 0;

	return s;
}

// create string copying given number of chars from c string
static inline String
S_Copy(char *cstr, size_t len) {
	return S_From(B_Wrap(cstr, len));
}

// transforms string to byte. Null-terminator will be preserved.
// Assumed that heap (not created by S_Wrap() string is used) but it will
// not panic at once (and that will also panic only if not null terminated is buffer
// passed!) as with S_FromBuffer if you violate memory segments
// so be careful. Original string will be nullified
static inline Buffer
S_ToBuffer(String *s) {
	Buffer b = {
		.len = s->len,
		.data = (void*) s->str,
	};

	S_Nullify(s);

	return b;
}

// free string memory and nullify it. just a reminder: don't try to free S_Wrap 
// strings
static inline void
S_Free(String *s) {
	free(s->str);
	S_Nullify(s);
}

#endif
