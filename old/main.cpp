
#include <aio.h>
#include <iostream>
#include <memory>
#include <signal.h>
#include <string>
#include <unistd.h>

#include "cmdln/process.h"
#include "cmdln/free.h"

#include "pool/pool.hpp"
#include "pqueue/pqueue.hpp"
#include "task/task.hpp"
#include "utils.hpp"
#include "misc/segv_backtrace.hpp"

pool::task::promise_type<bool>
func_pointer(std::shared_ptr<pool::pqueue<pool::task, pool::task::less>> que) {
	std::cout << "SUCCESS: "
			  << "hello from old function" << std::endl;

	que->push(pool::task(true));
	co_return true;
}

int main(int argc, char const* argv[]) {
	#ifdef DEBUGGING
	signal(SIGSEGV, segv_backtrace);
	#endif
	
	struct cmdln* flags = NULL;
	
	error = cmdln_process(&flags, argc, argv);
	
	if (!error)
	{
		using prio_queue = pool::pqueue<pool::task, pool::task::less>;
		
		auto queue = std::make_shared<prio_queue>(prio_queue());
		
		pool::tpool thread_pool = pool::tpool(flags->number_of_threads, queue);
		
		auto aio_file_read =
			[&](std::shared_ptr<prio_queue> que) -> pool::task::promise_type<bool> {
			char* buff[128] = {};
			FILE* fd = fopen(flags->input_path, "r");
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
			que->push(pool::task(true));

			co_return true;
		};

		auto file_name = [&](std::shared_ptr<prio_queue> que) -> pool::task::promise_type<bool> {
			std::cout << "SUCCESS: " << argv[1] << "\n";

			que->push(pool::task(1, func_pointer));
			que->push(pool::task(true));

			co_return true;
		};

		queue->push(pool::task(0, file_name));
		queue->push(pool::task(2, aio_file_read));
		queue->push(pool::task(true));

		// IMPORTANT:
		// __MUST__ push a task created with `pool::task(true)` as a signal to the thread it's done
		// working!
		thread_pool.spawn_tasks();
		
	}
	
	free_cmdln(flags);
	
	return 0;
}












