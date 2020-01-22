#include <stdio.h>
#include <stdlib.h>

/* There may be problems. GCC and Clang support almost all C11 features
 * but not all C libraries do. glibc supports C11 threads starting with 2.28.
 * If your platform uses older glibc you have no way to build the executable.
 * The best option is to try linking the executable against the musl C library.
 * To do that you can download and build musl library and compile with musl-gcc
 * (you should downgrade the -std compiler option to 'c11' in that case).
 */
#ifndef __STDC_NO_THREADS__
#include <threads.h>
#else
#error "your C library does not support native C11 threads"
#endif

int main()
{
	puts("Your C environment supports C11 threads. Congratulations!");
	return EXIT_SUCCESS;
}
