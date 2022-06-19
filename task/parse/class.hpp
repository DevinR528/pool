#pragma once

#include <string>

#include "../class.hpp"
#include "../kind.hpp"

namespace pool {
class parse_task: public task {
	std::string_view path;
  public:
	parse_task(std::string_view path);

	int process() override;

	~parse_task() override;
};

}  // namespace pool
