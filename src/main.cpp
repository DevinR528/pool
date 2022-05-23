#include "pool/pool.hpp"
#include "pqueue/pqueue.hpp"
#include "task/task.hpp"
#include "utils.hpp"

#include <aio.h>
#include <iostream>
#include <memory>
#include <signal.h>
#include <string>
#include <unistd.h>

void segv_backtrace(int sig) {
	void* array[10];
	size_t size;

	// get void*'s for all entries on the stack
	size = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, size, STDERR_FILENO);
	exit(1);
}

pool::task::promise_type<bool>
func_pointer(std::shared_ptr<pool::pqueue<pool::task, pool::task::less>> que) {
	std::cout << "SUCCESS: "
			  << "hello from old function" << std::endl;
	co_return true;
}

int main(int argc, char const* argv[]) {
	signal(SIGSEGV, segv_backtrace);

	using prio_queue = pool::pqueue<pool::task, pool::task::less>;

	auto queue = std::make_shared<prio_queue>(prio_queue());
	pool::tpool thread_pool = pool::tpool(4, queue);

	if (argc == 2) {
		auto aio_file_read =
			[&](std::shared_ptr<prio_queue> que) -> pool::task::promise_type<bool> {
			char* buff[128] = {};
			FILE* fd = fopen(argv[1], "r");
			if (fd == nullptr) { std::cout << "error: invalid file `" << argv[1] << "`\n"; }
			aiocb info = {};
			info.aio_fildes = fileno(fd);
			info.aio_buf = buff;
			info.aio_nbytes = 128;
			info.aio_reqprio = 0;
			info.aio_offset = 0;
			info.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
			aio_read(&info);

			int status = aio_error(&info);
			while (status == EINPROGRESS) {
				co_await std::suspend_always{};	 // let other threads make progress
				status = aio_error(&info);
			}

			if (status == ECANCELED) {
				std::cout << "CANCELED in aio read" << std::endl;
				co_return false;
			} else if (status != 0) {
				std::cout << "ERROR in aio read" << std::endl;
				co_return false;
			}

			std::cout << "SUCCESS: " << (char*)info.aio_buf << "\n";

			co_return true;
		};

		auto file_name = [&](std::shared_ptr<prio_queue> que) -> pool::task::promise_type<bool> {
			std::cout << "SUCCESS: " << argv[1] << "\n";

			que->push(pool::task(1, func_pointer));

			co_return true;
		};

		queue->push(pool::task(0, file_name));
		queue->push(pool::task(2, aio_file_read));

		auto threads = thread_pool.run_tasks();

	} else {
		std::cout << "error: must pass one argument\n";
		return 1;
	}

	return 0;
}
