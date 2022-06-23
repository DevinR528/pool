#include <execinfo.h>
#include <string>

#include "panic_with_trace.hpp"

namespace pool {

[[noreturn]] void panic_with_trace(std::string_view msg) {
	// #ifdef DEBUGGING
	void* array[32];
	size_t size;
	// get void*'s for all entries on the stack
	size = backtrace(array, 32);
	// print out all the frames to stderr
	fprintf(stderr, "Error: %s:\n", msg.data());
	char** names = backtrace_symbols(array, size);
	fprintf(stderr, "Backtrace:\n");
	for (size_t i = 0; i < size; i++) {
		fprintf(stderr, "%s\n", names[i]);
	}

	// #endif
	exit(errno);
}

}  // namespace pool
