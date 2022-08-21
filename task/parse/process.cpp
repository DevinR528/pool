#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../../enums/error.hpp"
#include "aio_read.hpp"
#include "class.hpp"
#include "tokenizer/class.hpp"
#include "tokenizer/scanner.hpp"

namespace pool {

task::promise_type<result<bool, pool_error>> parse_task::process(std::vector<std::unique_ptr<task>>& jobs) {
	this->print();

	scanner scan = scanner();

	// auto res = co_await scan.next_char();

	for (auto&& chunk : aio_file_read(jobs, this->path)) {
		std::cout << chunk << std::endl;
	}

	co_return true;
}

}  // namespace pool
