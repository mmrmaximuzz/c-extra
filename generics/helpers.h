#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>

#define STRINGIFY_HELPER(X) #X
#define STRINGIFY(X) STRINGIFY_HELPER(X)
static inline void panic_abort(const char *msg, const char *func, const char *lineno)
{
	fprintf(stderr, "%s in %s[%s]\n", msg, func, lineno);
	exit(EXIT_FAILURE);
}

#endif
