#include <string>

#include "../kind.hpp"
#include "class.hpp"

namespace pool {

parse_task::~parse_task() {
	task::~task();
}

}  // namespace pool
