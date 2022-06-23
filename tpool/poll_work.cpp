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
	// we came from the inner spin loop waiting for a job
	continue_outer:;
		{
			std::unique_lock<std::mutex> l(lock);

			while (!shutdown && jobs.empty()) {
				std::this_thread::yield();
				l.unlock();
				// We need to try again and not just spinlock this thread
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

		// Get the promise (an async value that will resolve sometime in the future)
		auto prom = job->process(this->jobs).hndl;
		// Make progress on resolving the value while we have not done so
		while (!prom.done()) {
			prom();

			if (prom.done()) {
				std::cout << "We got a: " << prom.promise().promise_value << std::endl;
				break;
			}
			// TODO: Give this unfinished async task to the scheduler to somehow
			// save it until the thing it's waiting on is ready (this will be info
			// passed from the task to the scheduler)
			else jobs.push_back(std::move(job));
		}
	}
}

}  // namespace pool
