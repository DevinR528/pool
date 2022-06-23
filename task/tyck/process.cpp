#include <coroutine>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "class.hpp"

namespace pool {

task::promise_type<pool_error> tyck_task::process(std::vector<std::unique_ptr<task>>& jobs) {
	this->print();
	co_return pool_error::e_success;
}

}  // namespace pool
