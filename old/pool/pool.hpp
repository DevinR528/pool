#pragma once

#include <concepts>
#include <coroutine>
#include <iostream>
#include <memory>
#include <thread>
#include <tuple>

#include "macro.hpp"
#include "pqueue/pqueue.hpp"
#include "task/task.hpp"

namespace pool {

class tpool {
  public:
	using prio_queue = pqueue<task, task::less>;

  private:
	size_t thread_count;
	size_t free_thread = 0;
	std::vector<std::tuple<task_status, std::thread>> threads;
	std::shared_ptr<prio_queue> queue;

  public:
	// Create a thread pool of `threads` number of threads and the given priority queue.
	tpool(size_t threads, std::shared_ptr<prio_queue> q) : thread_count(threads), queue(q) {}
	tpool(std::shared_ptr<prio_queue> q) :
		thread_count(std::thread::hardware_concurrency()),
		queue(q) {}

	tpool(tpool& other) = delete;
	// Keep move constructor
	tpool(tpool&& other) :
		thread_count(other.thread_count),
		threads(std::move(other.threads)),
		queue(std::move(other.queue)) {}

	~tpool() {
		for (auto&& [status, thread] : this->threads) {
			switch (status) {
				case ts_SUCCESS: {
					std::cout << "yay we made it!!!!" << std::endl;
					break;
				}
				case ts_FAILED: {
					std::cout << "ERROR: task failed" << std::endl;
					break;
				}
				case ts_WORKING: {
					std::cout << "Still working..." << std::endl;
					break;
				}
				default:
					panic_with_trace("unreachable task_status switch");
			}
			thread.join();
		}

		queue.~shared_ptr();
	}

	void run_tasks();

	void spawn_tasks();
};

}  // namespace pool
