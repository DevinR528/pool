
#include "pool.hpp"

#include <atomic>
#include <concepts>
#include <coroutine>
#include <iostream>
#include <optional>
#include <vector>

namespace pool {
	bool tpool::run_task() {
		std::cout << "threads:" << this->num_threads << "\n";

		auto task = this->queue->pop();

		std::cout << "has_task:" << task.has_value() << "\n";

		return task ? task.value()->call(this->queue) : false;
	}
}  // namespace pool
