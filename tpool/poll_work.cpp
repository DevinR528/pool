#include <concepts>
#include <coroutine>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../task/class.hpp"
#include "class.hpp"

namespace pool {

void thread_pool::poll_work() {
	std::unique_ptr<task> job;
	while (true) {
		{
			std::unique_lock<std::mutex> l(lock);

			while (!shutdown && jobs.empty()) {
				std::this_thread::yield();
				l.unlock();
				goto continue_outer;
			}

			if (jobs.empty()) {
				// No jobs to do and we are shutting down
				std::cout << "Thread " << std::this_thread::get_id() << " terminates" << std::endl;
				return;
			}

			std::cout << "Thread " << std::this_thread::get_id() << " does a job" << std::endl;
			job = std::move(jobs.back());
			jobs.pop_back();
		}

		// Do the job without holding any locks
		job->print();

		continue_outer:;
	}
}

}  // namespace pool
