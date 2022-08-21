#include <aio.h>
#include <coroutine>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "aio_read.hpp"
#include "class.hpp"

namespace pool {

task::promise_type<pool_error, std::string>
aio_file_read(std::vector<std::unique_ptr<task>>& que, std::string_view path) {
	char* buff[1024] = {};
	FILE* fd = fopen(path.data(), "r");
	if (fd == nullptr) { std::cout << "error: invalid file `" << path << "`\n"; }
	aiocb info = {};
	info.aio_fildes = fileno(fd);
	info.aio_buf = buff;
	info.aio_nbytes = 2048;
	info.aio_reqprio = 0;
	info.aio_offset = 0;
	info.aio_sigevent.sigev_notify = SIGEV_SIGNAL;

	size_t curr = 0;
	aio_read(&info);

	int status = aio_error(&info);
	while (status == EINPROGRESS) {
		std::string_view str = (char*)info.aio_buf;
		size_t amt_read = str.size();

		if (amt_read == 0) {
			// status = aio_error(&info);
			continue;
		}

		co_yield std::string(str, curr, amt_read);

		curr = amt_read;
		status = aio_error(&info);
	}
	// flush the rest, or yield the last chunk
	std::string_view str = (char*)info.aio_buf;
	size_t amt_read = str.size();
	co_yield std::string(str, curr, amt_read);

	if (status == ECANCELED) {
		std::cout << "CANCELED in aio read" << std::endl;
		co_return pool_error::e_canceled;
	} else if (status != 0) {
		std::cout << "ERROR in aio read" << std::endl;
		co_return pool_error::e_syscall_failed;
	}

	co_return pool_error::e_success;
};

}  // namespace pool
