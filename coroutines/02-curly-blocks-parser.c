/* Example of curly braces parser based on longjumps.
 * The original code can be found in the "Modern C" book by Jens Gustedt.
 */

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum brace_chars {
	LEFT_BRACE  = '{',
	RIGHT_BRACE = '}',
};

/* There are no any real programs with such block indent level */
#define MAX_RECURSION_LEVEL 30

/* All the exception cases for the recursive parser */
enum parser_state {
	NORMAL_EXECUTION = 0,
	TOO_MANY_LEFT_BRACES,
	TOO_MANY_RIGHT_BRACES,
	RECURSION_TOO_DEEP,
	END_OF_OUTPUT,
};

static const char *skipspace(const char *s)
{
	if (!s)
		return s;

	while (*s && isspace(*s))
		++s;

	return s;
}

static const char *skip_to_eol(const char *str, jmp_buf jump)
{
	if (putchar('\n') == EOF)
		longjmp(jump, END_OF_OUTPUT);

	return skipspace(str);
}

static const char *descend(const char *text,
			   volatile unsigned *recursion_level,
			   size_t len,
			   char buffer[len],
			   jmp_buf jump)
{
	/* check the current recursion level */
	if (*recursion_level > MAX_RECURSION_LEVEL)
		longjmp(jump, RECURSION_TOO_DEEP);

	/* go deeper and increment the current level */
	(*recursion_level)++;

newline:
	/* trying to read new line from stdin */
	while (!text || !*text) {
		text = skipspace(fgets(buffer, len, stdin));
		if (!text) {
			/* there are no text, check that all braces are closed */
			if (*recursion_level != 1)
				longjmp(jump, TOO_MANY_LEFT_BRACES);
			else
				goto ascend;
		}
	}

	/* print the current indent */
	unsigned indents_to_print = *recursion_level - 1;
	for (unsigned i = 0; i < indents_to_print; ++i)
		printf("----");

	/* parse the rest of the line */
	for (; text && *text; ++text) {
		switch (*text) {
		case LEFT_BRACE:
			text = skip_to_eol(text + 1, jump);
			text = descend(text, recursion_level, len, buffer, jump);
			text = skip_to_eol(text + 1, jump);
			goto newline;
		case RIGHT_BRACE:
			if (*recursion_level == 1)
				longjmp(jump, TOO_MANY_RIGHT_BRACES);
			else
				goto ascend;
		default:
			putchar(*text);
			break;
		}
	}

	goto newline;

ascend:
	(*recursion_level)--;
	return text;
}

#define BUFFER_SIZE 255

int main()
{
	char buffer[BUFFER_SIZE + 1] = { 0 };
	volatile unsigned recursion_level = 0;
	jmp_buf jump_buffer;

	switch (setjmp(jump_buffer)) {
	case NORMAL_EXECUTION:
		descend(0, &recursion_level, BUFFER_SIZE, buffer, jump_buffer);
		break;

	case TOO_MANY_LEFT_BRACES:
		fflush(stdout);
		fprintf(stderr, "error: too many (%d) left braces (%c)\n",
			recursion_level,
			LEFT_BRACE);
		break;

	case TOO_MANY_RIGHT_BRACES:
		fflush(stdout);
		fprintf(stderr, "error: too many (%d) right braces (%c)\n",
			recursion_level,
			RIGHT_BRACE);
		break;

	case END_OF_OUTPUT:
		fflush(stdout);
		fprintf(stderr, "error: EOF while printing on %d level\n",
			recursion_level);
		break;

	default:
		fflush(stdout);
		fprintf(stderr, "unknown error on %d level\n", recursion_level);
		break;
	}

	return EXIT_SUCCESS;
}
