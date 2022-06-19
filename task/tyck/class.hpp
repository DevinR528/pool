#pragma once

#include <string>

#include "../class.hpp"
#include "../kind.hpp"

namespace pool {
class tyck_task: public task {
  public:
	tyck_task();

	int process() override;

	~tyck_task() override;
};

}  // namespace pool
