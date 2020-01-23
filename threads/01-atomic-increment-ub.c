/* This code sample contains code with UB on non-synchronized counter updates.
 * Please NEVER do that in your real production code. */

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

/* The bug is to use raw volatile objects that don't have any
 * happened-before semanthics. We addded `volatile` keyword here to force
 * the compiler to _really_ update the variable on each iteration.
 * Leads to code like that:
 * - mov    (%rdi),%rax
 * - add    $0x1,%rax
 * - mov    %rax,(%rdi)
 */
int buggy_counter_updater(void *arg)
{
	volatile size_t *counter = arg;
	for (size_t i = 0; i < MAX_ITERATIONS; ++i)
		++(*counter);

	return EXIT_SUCCESS;
}

int main()
{
	puts("Your C environment supports C11 threads. Congratulations!");

	size_t counter = 0;
	thrd_t thread1 = { 0 };
	thrd_t thread2 = { 0 };

	/* we ignore the error codes here for simplicity */
	thrd_create(&thread1, buggy_counter_updater, &counter);
	thrd_create(&thread2, buggy_counter_updater, &counter);
	thrd_join(thread1, &(int){0});
	thrd_join(thread2, &(int){0});

	printf("the counter value is: %zu (BUGGY)\n", counter);
	return EXIT_SUCCESS;
}
