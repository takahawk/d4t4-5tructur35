#include "string_.h"

#include <stdio.h>

String
S_Format(char *fmt, ...) {
	String s;
	// TODO: implement embedding String as %s
	va_list args;
	va_start(args, fmt);

	s.len = vsnprintf(NULL, 0, fmt, args);

	va_end(args);
	if (s.len == 0) {
		return S_Null();
	}

	// null terminator
	s.len++;

	s.str = malloc(s.len);
	va_start(args, fmt);
	vsprintf(s.str, fmt, args);

	va_end(args);

	return s;
}

