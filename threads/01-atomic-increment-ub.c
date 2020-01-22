#include <stdio.h>
#include <stdlib.h>

#ifndef __STDC_NO_THREADS__
#include <threads.h>
#else
#error "Your build environment does not support C11 threads"
#endif

int main()
{
	puts("If you can see this message, then your C environment "
	     "supports C11 threads");
	return EXIT_SUCCESS;
}
