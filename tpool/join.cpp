#include <concepts>
#include <coroutine>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "class.hpp"

namespace pool {

int thread_pool::join() {
	{
		// Unblock any threads and tell them to stop
		std::unique_lock<std::mutex> l(lock);
		shutdown = true;
	}

	// Wait for all threads to stop
	std::cout << "Joining threads" << std::endl;
	for (auto& thread : threads)
		thread.join();

	return 0;
}

}  // namespace pool
