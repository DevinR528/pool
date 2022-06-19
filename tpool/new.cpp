#include <concepts>
#include <coroutine>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "class.hpp"

namespace pool {

thread_pool::thread_pool(std::vector<std::unique_ptr<task>> queue) :
	thread_pool(std::move(queue), std::thread::hardware_concurrency()) {}

thread_pool::thread_pool(std::vector<std::unique_ptr<task>> queue, uint8_t threads) :
	num_threads(threads),
	jobs(std::move(queue)) {
	this->threads.reserve(this->num_threads);
	for (int i = 0; i < this->num_threads; ++i)
		this->threads.emplace_back(std::bind(&thread_pool::poll_work, this));
}

}  // namespace pool
