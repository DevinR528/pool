#include <iostream>
#include <string>

#include "../kind.hpp"
#include "class.hpp"

namespace pool {

tyck_task::tyck_task() noexcept : task(task_kind::tk_typecheck) {}

tyck_task::tyck_task(tyck_task&& t) noexcept {
	this->kind = std::move(t.kind);
	this->color = std::move(t.color);
	std::cout << "Moved tyck: " << this->color << " " << this->kind << std::endl;
}

}  // namespace pool
