#pragma once

#include <string>

#include "../class.hpp"
#include "../kind.hpp"

namespace pool {
class parse_task: public task {
  public:
	parse_task(std::string_view path) : task(task_kind::tk_parse) {}

	int compare(const struct task*, const struct task*);

	int process();

	void print();
};

}  // namespace pool
