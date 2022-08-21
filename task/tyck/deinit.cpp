#include <string>

#include "../kind.hpp"
#include "class.hpp"

namespace pool {

tyck_task::~tyck_task() {
	task::~task();
}

}  // namespace pool
