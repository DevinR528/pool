#include <coroutine>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "class.hpp"
#include "../class.hpp"
#include "../../misc/result.hpp"

namespace pool {

task::promise_type<result<bool, pool_error>> tyck_task::process(std::vector<std::unique_ptr<task>>& jobs) {
	this->print();
	co_return true;
}

}  // namespace pool
