/* This example demonstates useful function which helps programmer to
 * investigate messy type transformations in C arithmetics */

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

/* C numeric types enumeration */
enum GType {
	NOT_IN_THE_LIST,
	U8,
	U16,
	U32,
	U64,
	I8,
	I16,
	I32,
	I64,
	F32,
	F64,
	FLONG,
	GTYPEMAX,
};

#define ARRAY_INIT_TYPESTRING(TYPE) [(TYPE)] = STRINGIFY(TYPE)

static const char *type_strings[] = {
	ARRAY_INIT_TYPESTRING(NOT_IN_THE_LIST),
	ARRAY_INIT_TYPESTRING(U8),
	ARRAY_INIT_TYPESTRING(U16),
	ARRAY_INIT_TYPESTRING(U32),
	ARRAY_INIT_TYPESTRING(U64),
	ARRAY_INIT_TYPESTRING(I8),
	ARRAY_INIT_TYPESTRING(I16),
	ARRAY_INIT_TYPESTRING(I32),
	ARRAY_INIT_TYPESTRING(I64),
	ARRAY_INIT_TYPESTRING(F32),
	ARRAY_INIT_TYPESTRING(F64),
	ARRAY_INIT_TYPESTRING(FLONG),
};

const char *get_type_string(enum GType type)
{
	assert(0 <= type && type < GTYPEMAX);
	return type_strings[type];
}

/* The [u]int_{N}_t types are just typedefs to plain C numeric types.
 * It creates lots of oddities on different platforms, but we are sticked
 * to the exact width types just to make sure they are different.
 */
#define GET_TYPE(X)				\
	_Generic((X),				\
		 uint8_t: U8,			\
		 uint16_t: U16,			\
		 uint32_t: U32,			\
		 uint64_t: U64,			\
		 int8_t: I8,			\
		 int16_t: I16,			\
		 int32_t: I32,			\
		 int64_t: I64,			\
		 float: F32,			\
		 double: F64,			\
		 long double: FLONG,		\
		 default: NOT_IN_THE_LIST)

#define PRINT_TYPE(X)							\
	do {								\
		printf("type of (" STRINGIFY(X) ") is %s\n",		\
		       get_type_string(GET_TYPE(X)));			\
	} while (0)

int main()
{
	/* do not compile on rare platforms with strange ints */
	static_assert(sizeof(int) == 4, "your int is not 32 bits long");
	static_assert((-1 & 3) == 3, "your signed ints are not two's complement");

	/* char literals are ints */
	PRINT_TYPE('!');

	/* chars are not int8_t and not uint8_t */
	char char_a = 'a';
	PRINT_TYPE(char_a);
	puts("");

	/* decimal literals are ints */
	PRINT_TYPE(1);

	/* hexadecimal literals are signed if not greater than INT_MAX ... */
	PRINT_TYPE(0x01234567);

	/* ... but are unsigned otherwise */
	PRINT_TYPE(0xdeadf00d);
	puts("");

	/* INT_MIN value written as a literal is in fact int64_t */
	int int_min = -2147483648;
	PRINT_TYPE(int_min);
	PRINT_TYPE(-2147483648);
	puts("");

	/* what about size_t/ptrdiff_t/intptr_t? */
	size_t size = 42;
	ptrdiff_t ptrdiff = &size - &size;
	intptr_t intptr = (intptr_t) &size;
	PRINT_TYPE(size);
	PRINT_TYPE(ptrdiff);
	PRINT_TYPE(intptr);
	puts("");

	/* "true" literal is a macro to literal "1" (which is int) */
	PRINT_TYPE(true);

	/* true bool object is not a numeric type */
	bool boolean_obj = true;
	PRINT_TYPE(boolean_obj);
	puts("");

	/* NULL is usually a macro to (void*)0 ...
	 * ... but on some distrubutions it is a typedef to 0 (which is int)
	 * avoid using NULL in type-sensitive macros */
	PRINT_TYPE(NULL);
	puts("");

	/* let's do some arithmetics */

	/* chars are expanded but sign may change */
	signed char char_obj = 'x';
	unsigned char uchar_obj = 'x';
	PRINT_TYPE(char_obj + 1); /* trivial */
	PRINT_TYPE(char_obj + 1u); /* sign lost */
	PRINT_TYPE(uchar_obj + 1); /* got sign */
	PRINT_TYPE(uchar_obj + 1u); /* trivial */
	puts("");

	/* shorts are like chars in this case */
	signed short short_obj = 10000;
	unsigned short ushort_obj = 10000;
	PRINT_TYPE(short_obj + 1);
	PRINT_TYPE(short_obj + 1u);
	PRINT_TYPE(ushort_obj + 1);
	PRINT_TYPE(ushort_obj + 1u);
	puts("");

	/* ints are wide enough and need expansion only when added to longs */
	signed int_obj = 1000000;
	unsigned uint_obj = 1000000;
	PRINT_TYPE(int_obj + 1);
	PRINT_TYPE(int_obj + 1u);
	PRINT_TYPE(uint_obj + 1); /* hahaha, the logic has changed */
	PRINT_TYPE(uint_obj + 1u);
	puts("");

	/* floats are expanded to the widest type */
	PRINT_TYPE(1.0f + 1.0f);
	PRINT_TYPE(1.0f + 1.0);
	PRINT_TYPE(1.0f + 1.0L);
	PRINT_TYPE(1.0 + 1.0);
	PRINT_TYPE(1.0L + 1.0);
	PRINT_TYPE(1.0L + 1.0L);

	return EXIT_SUCCESS;
}
