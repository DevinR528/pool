#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../class.hpp"
#include "../kind.hpp"
#include "../../enums/error.hpp"

namespace pool {
class tyck_task: public task {
  public:
	tyck_task() noexcept;
	tyck_task(const tyck_task&) = delete;
	tyck_task(tyck_task&&) noexcept;

	tyck_task& operator=(tyck_task&& other) {
		this->kind = std::move(other.kind);
		this->color = std::move(other.color);
		return *this;
	}

	task::promise_type<pool_error> process(std::vector<std::unique_ptr<task>>&) override;

	~tyck_task() override;
};

}  // namespace pool
