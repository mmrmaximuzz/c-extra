#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

enum GeneratorState {
	SET_GENERATOR = 0,
	HAS_ELEMENTS,
	STOP_ITERATION,
};

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
	if (argc != 3) {
		printf("usage: %s START STOP\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	size_t maxval = atoi(argv[2]);
	volatile size_t counter = atoi(argv[1]);
	printf("simulating range: [%zu, %zu)\n", counter, maxval);

	jmp_buf generator_jmp;
	switch (setjmp(generator_jmp)) {
	case SET_GENERATOR:
		puts("generator started");
		printf("counter = %zu\n", counter);
		range(&counter, maxval, generator_jmp);
		break;
	case HAS_ELEMENTS:
		printf("counter = %zu\n", counter);
		range(&counter, maxval, generator_jmp);
		break;
	case STOP_ITERATION:
		puts("generator is exhausted");
		break;
	}

	return EXIT_SUCCESS;
}
