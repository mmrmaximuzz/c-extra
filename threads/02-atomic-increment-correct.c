/* This code sample contains code with good synchronization between threads
 * using the atomic variables */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* There may be problems. GCC and Clang support almost all C11 features
 * but not all C libraries do. glibc supports C11 threads starting with 2.28.
 * If your platform uses older glibc you have no way to build the executable.
 * The best option is to try linking the executable against the musl C library.
 * To do that you can download and build musl library and compile with musl-gcc.
 */
#ifndef __STDC_NO_THREADS__
#include <threads.h>
#else
#error "your C library does not support native C11 threads"
#endif

#define MAX_ITERATIONS 50000

/* Use atomics and have no problems */
int correct_counter_updater(void *arg)
{
	_Atomic(size_t) *counter = arg;
	for (size_t i = 0; i < MAX_ITERATIONS; ++i)
		/* Note that we use here plain ++ operator without any problems.
		 * The _Atomic declaration make all this work for us. */
		++(*counter);

	return EXIT_SUCCESS;
}

int main()
{
	puts("Your C environment supports C11 threads. Congratulations!");

	_Atomic(size_t) counter = 0;
	thrd_t thread1 = { 0 };
	thrd_t thread2 = { 0 };

	/* we ignore the error codes here for simplicity */
	thrd_create(&thread1, correct_counter_updater, &counter);
	thrd_create(&thread2, correct_counter_updater, &counter);
	thrd_join(thread1, &(int){0});
	thrd_join(thread2, &(int){0});

	printf("the counter value is: %zu (CORRECT)\n", counter);
	return EXIT_SUCCESS;
}
