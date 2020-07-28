#include <stdio.h>
#include <stdlib.h>

#ifndef __STDC_NO_THREADS__
#include <threads.h>
#else
#error "your environment does not support C11 threads"
#endif

/* Plain int triple to pass arguments into thread body functions. */
struct triple {
	int x;
	int y;
	int z;
};

/* We declare this variable in a special thread storage class.
 * All threads have a copy for that variable, and it works like
 * a global variable for the thread.
 */
thread_local volatile int my_local = 0;

/* Threads init their local storage value and update it in a cycle. */
int local_tester(void *arg)
{
	struct triple *p = arg;
	int thread_id = p->x;
	int init_value = p->y;
	int n_iterations = p->z;

	my_local = init_value;
	for (int i = 0; i < n_iterations; ++i) {
		printf("thread %d: updates value and now it is %d\n",
		       thread_id,
		       my_local);
		++my_local;
	}

	return EXIT_SUCCESS;
}

int main()
{
	/* let's init 3 threads and see how they do manage their local ints */
	struct triple thread1_arg = {
		.x = 1,
		.y = 0,
		.z = 5000,
	};

	struct triple thread2_arg = {
		.x = 2,
		.y = 10000,
		.z = 5000,
	};

	struct triple thread3_arg = {
		.x = 3,
		.y = 20000,
		.z = 5000,
	};

	/* run all the threads */
	thrd_t thread1, thread2, thread3;
	thrd_create(&thread1, local_tester, &thread1_arg);
	thrd_create(&thread2, local_tester, &thread2_arg);
	thrd_create(&thread3, local_tester, &thread3_arg);
	thrd_join(thread1, &(int){0});
	thrd_join(thread2, &(int){0});
	thrd_join(thread3, &(int){0});

	return EXIT_SUCCESS;
}
