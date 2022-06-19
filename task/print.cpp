#include <string>
#include <iostream>

#include "class.hpp"
#include "kind.hpp"

namespace pool {

void task::print() {
	#ifdef DEBUGGING
	std::cout << "\x1B[" << (this->kind + 31) << "m" << TASK_KIND_NAMES[this->kind] << "\x1B[0m" << std::endl;
	#endif
}

}  // namespace pool
