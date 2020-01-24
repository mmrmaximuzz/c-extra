#include <stdio.h>
#include <stdlib.h>

#ifndef __STDC_NO_THREADS
#include <threads.h>
#else
#error "your environment does not support C11 threads"
#endif

int main()
{
	puts("Hello world!");
	return EXIT_SUCCESS;
}
