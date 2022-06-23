#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../class.hpp"
#include "../kind.hpp"

namespace pool {
class parse_task: public task {
	std::string_view path;

  public:
	parse_task(std::string_view path) noexcept;
	parse_task(const parse_task&) = delete;
	parse_task(parse_task&&) noexcept;

	parse_task& operator=(parse_task&& other) {
		this->kind = std::move(other.kind);
		this->color = std::move(other.color);
		return *this;
	}

	task::promise_type<pool_error> process(std::vector<std::unique_ptr<task>>&) override;

	~parse_task() override;
};

}  // namespace pool
