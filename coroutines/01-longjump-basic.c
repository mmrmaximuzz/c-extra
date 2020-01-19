/* Simple example with pseudo-python range generator */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

enum GeneratorState {
	SET_GENERATOR = 0, /* reserved value for the first setjmp call */
	HAS_ELEMENTS,
	STOP_ITERATION,
};

/* Main range generator body.
 * Volatile is used to prevent memory access optimizations (long jumps
 * are opaque for the compiler). _Noreturn is used to conform C11 standard.
 */
_Noreturn void range(volatile size_t *ptr, size_t maxval, jmp_buf gen_jmp)
{

	size_t value = *ptr;
	if (value >= maxval - 1)
		longjmp(gen_jmp, STOP_ITERATION);

	*ptr = value + 1;
	longjmp(gen_jmp, HAS_ELEMENTS);
}

int main(int argc, char *argv[argc])
{
	/* cmd line args check */
	if (argc != 3) {
		printf("usage: %s START STOP\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	size_t maxval = atoi(argv[2]);
	volatile size_t counter = atoi(argv[1]);
	printf("simulating range: [%zu, %zu)\n", counter, maxval);

	jmp_buf generator_jmp;
	switch (setjmp(generator_jmp)) {

	/* You get here when you called setjmp for the first time.
	 * It is impossible by the standard to get 0 value twice.
	 */
	case SET_GENERATOR:
		puts("generator started");
		printf("counter = %zu\n", counter);
		range(&counter, maxval, generator_jmp);
		break;

	/* There are maybe some extra elements in the generator
	 * so let's call `range` one more time.
	 */
	case HAS_ELEMENTS:
		printf("counter = %zu\n", counter);
		range(&counter, maxval, generator_jmp);
		break;

	/* The generator is exhausted. Just continue execution. */
	case STOP_ITERATION:
		puts("generator is exhausted");
		break;
	}

	return EXIT_SUCCESS;
}
