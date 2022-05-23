
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
void tpool::run_tasks() {
	auto task = this->queue->pop();
	if (!task.has_value()) { return false; }

	int32_t prio = task.value()->priority();
	if (!this->set_task(std::move(task.value()))) { return true; }
	while (true) {
		auto next = this->queue->pop();
		if (!next.has_value()) { return false; }

		if (prio == next.value()->priority()) {
			if (!this->set_task(std::move(next.value()))) { return true; }
		} else {
			this->queue->push(*next.value());
			break;
		}
	}

	std::cout << "HERE" << std::endl;
	for (auto&& trd : this->threads) {
		task_status res = std::get<0>(trd);
		if (res == ts_FAILED || res == ts_SUCCESS) {
			std::cout << "FINISHED A TASK" << std::endl;
			std::get<1>(trd)->join();
			// std::get<1>(trd)->~thread();
		}
	}

	return true;
}

// `tpool::set_task` returns `optional::none` if there are no threads left in the thread pool, it
// will also return the task it tried to put on the thread pool into the priority queue.
std::optional<std::shared_ptr<std::thread>> tpool::set_task(task::task_ptr task) {
	std::cout << "set_task " << this->free_thread << std::endl;
	if (this->free_thread >= this->thread_count) {
		this->queue->push(*task);
		return {};
	}

	task_status done = ts_WORKING;
	auto promise = std::make_shared<std::thread>([&]() -> bool {
		// Get the promise (an async value that will resolve sometime in the future)
		auto prom = task->call(this->queue).hndl;

		// Make progress on resolving the value while we have not done so
		while (!prom.done()) {
			prom();
			std::this_thread::yield();
		}

		std::cout << prom.promise().promise_value << std::endl;
		// Now that we have resolved the promise value, use it
		auto res = prom.promise().promise_value;


		done = res ? ts_SUCCESS : ts_FAILED;

	std::cout << "HERE " << (done == ts_SUCCESS ? "true" : "false") << std::endl;

		return res;
	});
	this->threads[this->free_thread] = std::make_tuple(done, promise);

	auto trd = std::get<1>(this->threads[this->free_thread]);
	trd->detach();
	this->free_thread += 1;
	return std::optional<std::shared_ptr<std::thread>>(trd);
}

// bool tpool::run_task() {
// 	std::cout << "threads: " << this->threads.size() << std::endl;

// 	auto task = this->queue->pop();

// 	if (!task.has_value()) { return false; }

// 	// Get the promise (an async value that will resolve sometime in the future)
// 	auto prom = task.value()->call(this->queue).hndl;

// 	// Make progress on resolving the value while we have not done so
// 	while (!prom.done()) { prom(); }

// 	std::cout << prom.promise().promise_value << std::endl;
// 	// Now that we have resolved the promise value, use it
// 	return prom.promise().promise_value;
// }
}  // namespace pool
