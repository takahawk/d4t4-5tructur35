#ifndef STRING_H_
#define STRING_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "buffer.h"

#define CONV_BUFFER_SIZE 20
#define S_From(X, ...) _Generic((X), \
	Buffer: S_FromBuffer,   \
	Buffer*: S_FromBufferP, \
	int: S_FromInt,         \
	bool: S_FromBool,       \
	float: S_FromFloat,     \
	char*: S_Wrap           \
	) (X __VA_OPT__(,) __VA_ARGS__)

#define S_Copy(X, ...) _Generic(X, \
	String: _S_CopyString, \
	char*: _S_CopyCharP    \
	) (X)


// in essense a variant of buffer. functions from this unit are supposed
// to ensure null-termination and that it is preserved 
typedef struct String {
	size_t len;
	char* str;
} String;

static inline Buffer
S_AsBuffer(String s);

static inline int
S_Cmp(String a, String b);

static inline String
_S_CopyCharP(char*);

static inline String
_S_CopyString(String);

static inline String
S_CopyN(char *cstr, size_t len);

static inline bool
S_Equals(String a, String b);

static inline void
S_Free(String *s);

static inline String
S_FromBool(bool b);

static inline String
S_FromBuffer(Buffer buf);

static inline String
S_FromBufferP(Buffer *buf);

static inline String
S_FromFloat(float f);

static inline String
S_FromInt(int i);

static inline bool
S_IsNull(String s);

static inline String
S_Null();

static inline void
S_Nullify(String *s);

static inline Buffer
S_ToBuffer(String *s);

static inline String
S_Wrap(char* str);

String
S_Format(char *fmt, ...);

static inline Buffer
S_AsBuffer(String s) {
	return (Buffer) {
		.data = s.str,
		.len = s.len
	};
}

static inline int
S_Cmp(String a, String b) {
	return B_Cmp((Buffer) {
			.len = a.len,
			.data = a.str,
			},
		     (Buffer) {
		     	.len = b.len,
			.data = b.str,
		     });
}

// copies the whole string up to null terminator (including it)
static inline String
_S_CopyCharP(char *cstr) {
	size_t len = strlen(cstr);
	return S_CopyN(cstr, len);
}

static inline String
_S_CopyString(String s) {
	return S_CopyN(s.str, s.len);
}

// create string copying given number of chars from c string
static inline String
S_CopyN(char *cstr, size_t len) {
	return S_From(B_From(cstr, len));
}

static inline bool
S_Equals(String a, String b) {
	return S_Cmp(a, b) == 0;
}

// basically just wraps C string. memory is presumed to be handled elsewhere
// without triggering S_Free. It is assumed that given string is already null
// terminated. In other case use S_From()
static inline String
S_Wrap(char* str) {
	return (String) {
		// with null terminator
		.len = strlen(str) + 1,
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
// if needed). it is assumed that it is buffer that was allocated on heap
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
	B_Nullify(buf);

	return s;
}

static inline String
S_FromInt(int i) {
	char convBuf[CONV_BUFFER_SIZE];
	sprintf(convBuf, "%d", i);
	return S_Copy(convBuf);
}

static inline String
S_FromFloat(float f) {
	char convBuf[CONV_BUFFER_SIZE];
	sprintf(convBuf, "%f", f);
	return S_Copy(convBuf);
}

static inline String
S_FromBool(bool b) {
	return S_Copy(b ? "true" : "false");
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
