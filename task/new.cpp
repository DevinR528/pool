#include <iostream>

#include "class.hpp"
#include "kind.hpp"

namespace pool {

task::task(task_kind kind) :
	kind(kind),
	color(compute_color((double)kind / (double)task_kind::tk_count)) {
	std::cout << this->color << " " << this->kind << std::endl;
}

}  // namespace pool
