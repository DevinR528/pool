#include <iostream>

#include "class.hpp"
#include "kind.hpp"

namespace pool {

task::task() {}

/// Initializing constructor
task::task(task_kind kind) noexcept :
	kind(kind),
	color(compute_color((double)kind / (double)task_kind::tk_count)) {
	std::cout << this->color << " " << this->kind << std::endl;
}

/// Move constructor
task::task(task&& t) noexcept : kind(t.kind), color(std::move(t.color)) {
	std::cout << "Moved base task: " << this->color << " " << this->kind << std::endl;
}

}  // namespace pool
