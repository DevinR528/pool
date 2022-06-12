
#include "segv_backtrace.hpp"

#include <execinfo.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <string>

[[noreturn]] void segv_backtrace(int sig) {
	void* array[10];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}
