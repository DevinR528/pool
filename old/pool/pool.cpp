
#include "pool.hpp"

#include <atomic>
#include <concepts>
#include <coroutine>
#include <iostream>
#include <optional>
#include <thread>
#include <tuple>
#include <vector>

namespace pool {
void tpool::spawn_tasks() {
	this->threads.reserve(this->thread_count);
	for (size_t i = 0; i < this->thread_count; ++i)
		this->threads.emplace_back(
			std::make_tuple(ts_WORKING, std::thread(std::bind(&tpool::run_tasks, this))));
}

// `tpool::set_task` returns `optional::none` if there are no threads left in the thread pool, it
// will also return the task it tried to put on the thread pool into the priority queue.
void tpool::run_tasks() {
	std::cout << "set_task " << this->free_thread << std::endl;

	task_status done = ts_WORKING;
	while (true) {
		std::optional<task::task_ptr> opt_task;
		// We are waiting for work
		while (!(opt_task = this->queue->pop()).has_value()) {
			std::this_thread::yield();
		}
		// We have finished everything and now this thread can exit
		if (opt_task.value()->is_done()) { return; }

		auto task = std::move(opt_task.value());
		// Get the promise (an async value that will resolve sometime in the future)
		auto prom = task->call(this->queue).m_hndl;

		// Make progress on resolving the value while we have not done so
		while (!prom.done()) {
			prom();
			// TODO: save this prom with an id and pop a new task
			std::this_thread::yield();
		}

		std::cout << prom.promise().m_promise_value << std::endl;
		// Now that we have resolved the promise value, use it
		auto res = prom.promise().m_promise_value;

		done = res ? ts_SUCCESS : ts_FAILED;

		std::cout << "Task done:  " << (done == ts_SUCCESS ? "true" : "false") << std::endl;
	}
}

// bool tpool::run_task() {
// 	std::cout << "threads: " << this->threads.size() << std::endl;

// 	auto task = this->queue->pop();

// 	if (!task.has_value()) { return false; }

// 	// Get the promise (an async value that will resolve sometime in the future)
// 	auto prom = task.value()->call(this->queue).m_hndl;

// 	// Make progress on resolving the value while we have not done so
// 	while (!prom.done()) { prom(); }

// 	std::cout << prom.promise().m_promise_value << std::endl;
// 	// Now that we have resolved the promise value, use it
// 	return prom.promise().m_promise_value;
// }
}  // namespace pool
