#include <stdio.h>
#include <stdlib.h>

#ifndef __STDC_NO_THREADS
#include <threads.h>
#else
#error "your environment does not support C11 threads"
#endif

/* Name for the log path. */
static const char *log_path = "./log.log";

/* We use that global variable to share logging file object between threads. */
static FILE *global_logging_file;
static once_flag logging_init_flag = ONCE_FLAG_INIT;

/* Init/deinit functions for the logger. */
static void init_logger(void)
{
	global_logging_file = fopen(log_path, "w");
	if (global_logging_file) {
		fprintf(stderr, "opened log file at: %s\n", log_path);
	} else {
		fprintf(stderr, "cannot open log file at %s\n", log_path);
		exit(EXIT_FAILURE);
	}
}

static void deinit_logger(void)
{
	if (global_logging_file) {
		fprintf(stderr, "closing log file\n");
		fclose(global_logging_file);
	}
}

/* Threads use call_once to ensure the logger is initialized. */
static int logger(void *arg)
{
	int *thread_id = arg;

	/* Init logging file */
	call_once(&logging_init_flag, init_logger);

	/* at this point logging file is already initialized */
	for (size_t i = 0; i < 50; ++i) {
		fprintf(global_logging_file,
			"thread %d: iteration %zu\n",
			*thread_id, i);
	}
	return EXIT_SUCCESS;
}

int main()
{
	/* registering deinit callback on program termination */
	atexit(deinit_logger);

	/* launching the threads */
	thrd_t thread1, thread2, thread3;
	thrd_create(&thread1, logger, &(int){1});
	thrd_create(&thread2, logger, &(int){2});
	thrd_create(&thread3, logger, &(int){3});
	thrd_join(thread1, NULL);
	thrd_join(thread2, NULL);
	thrd_join(thread3, NULL);

	return EXIT_SUCCESS;
}
