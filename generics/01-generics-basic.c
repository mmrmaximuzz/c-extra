/* This code sample demonstrates how _Generic feature can be used in
 * type-generic macros to provide extra typing check for the input argument.
 */

#include <stdio.h>
#include <stdlib.h>

void print_float(float x)
{
	printf("%.7e", x);
}

void print_double(double x)
{
	printf("%.14e", x);
}

/* The exact precision of long double is platform-dependant.
 * Let's try to do the best and print as more digits as we can.
 */
void print_long_double(long double x)
{
	printf("%.28Le", x);
}

/* There are some typing error, aborting the program. */
#define STRINGIFY_HELPER(X) #X
#define STRINGIFY(X) STRINGIFY_HELPER(X)
void panic(const char *msg, const char *func, const char *lineno)
{
	fprintf(stderr, "%s in %s[%s]\n", msg, func, lineno);
	exit(EXIT_FAILURE);
}

#define panic_on_bad_expr(BADEXPR)					\
	panic("expression (" STRINGIFY(BADEXPR) ") is non-float",	\
	      __func__,							\
	      STRINGIFY(__LINE__))

#define fpuprintf(X)					\
	_Generic((X),					\
		 float: print_float(X),			\
		 double: print_double(X),		\
		 long double: print_long_double(X),	\
		 default: panic_on_bad_expr(X))

int main()
{
	float x = 2.71828182845904523536028747135266L;
	fpuprintf(x);
	puts("");

	double y = 2.71828182845904523536028747135266L;
	fpuprintf(y);
	puts("");

	long double z = 2.71828182845904523536028747135266L;
	fpuprintf(z);
	puts("");

	int foo = -100;
	fpuprintf(foo); /* abort here */

	/* unreachable */
	return EXIT_SUCCESS;
}
