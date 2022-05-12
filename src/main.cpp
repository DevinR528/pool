
#include "pool/pool.hpp"
#include "pqueue/pqueue.hpp"
#include "task/task.hpp"

#include <iostream>
#include <liburing.h>
#include <memory>

int main(int argc, char const* argv[]) {
	using prio_queue = pool::pqueue<pool::task, pool::task::less>;

	// pqueue has to be cheap to copy since we can hide all the syncronization
	auto p = prio_queue(10);
	auto queue = std::make_shared<prio_queue>(10);
	pool::tpool thread_pool = pool::tpool(4, queue);

	if (argc == 2) {
		auto read_vowls = [&](std::shared_ptr<prio_queue> que) {
			std::cout << argv[1] << "\n";
			return true;
		};
		queue->push(pool::task(0, read_vowls));
		while (thread_pool.run_task()) {};
	} else {
		std::cout << "error: must pass one argument\n";
		return 1;
	}

	return 0;
}
