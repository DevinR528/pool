#pragma once

#include <string>

namespace pool {
enum pool_error {
	e_success,
	e_syscall_failed,
	e_bad_cmdline_args,
	e_out_of_memory,
	e_bad_input_file,
	e_canceled,

	number_of_errors,
};

}
