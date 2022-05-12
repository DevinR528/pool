#pragma once

#include "macro.hpp"

#include "pqueue/pqueue.hpp"
#include "task/task.hpp"

#include <concepts>
#include <coroutine>
#include <iostream>
#include <liburing.h>
#include <memory>

namespace pool {

class tpool {
	public:
	using prio_queue = pqueue<task, task::less>;

	private:
	size_t num_threads;
	std::shared_ptr<prio_queue> queue;


  public:
	tpool(tpool& other) = delete;
	tpool(tpool&& other) = delete;
	tpool(size_t nt, std::shared_ptr<prio_queue> q) : num_threads(nt), queue(q) {}
	~tpool() {
		queue.~shared_ptr();
	}

	bool run_task();

};

}  // namespace pool
