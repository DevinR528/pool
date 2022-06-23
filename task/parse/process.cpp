#include <memory>
#include <string>
#include <vector>
#include <future>
#include <iostream>

#include "class.hpp"
#include "aio_read.hpp"
#include "../../enums/error.hpp"

namespace pool {

task::promise_type<pool_error> parse_task::process(std::vector<std::unique_ptr<task>>& jobs) {
	this->print();

	for (auto &&chunk : aio_file_read(jobs, this->path))
	{
		std::cout << chunk << std::endl;
	}

	co_return pool_error::e_success;
}

}  // namespace pool
