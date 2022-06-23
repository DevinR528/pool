#include <iostream>
#include <string>

#include "../kind.hpp"
#include "class.hpp"

namespace pool {

parse_task::parse_task(std::string_view path) noexcept : task(task_kind::tk_parse), path(path) {}

parse_task::parse_task(parse_task&& t) noexcept : path(std::move(t.path)) {
	this->kind = std::move(t.kind);
	this->color = std::move(t.color);
	std::cout << "Moved parse: " << this->color << " " << this->kind << std::endl;
}

}  // namespace pool
