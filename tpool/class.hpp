#pragma once

#include <concepts>
#include <coroutine>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../misc/panic_with_trace.hpp"
#include "../task/class.hpp"

namespace pool {
class thread_pool {
  public:
	uint8_t num_threads;
	std::mutex lock;
	bool shutdown;
	std::vector<std::thread> threads;
	std::vector<std::unique_ptr<task>> jobs;

	thread_pool(std::vector<std::unique_ptr<task>>);
	thread_pool(std::vector<std::unique_ptr<task>>, uint8_t threads);

	int join();

  private:
	void poll_work();
};

}  // namespace pool
