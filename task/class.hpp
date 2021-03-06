#pragma once

#include <string>

#include "kind.hpp"

namespace pool {
class task {
  public:
	task_kind kind;
	std::string color;

	task() = delete;
	task(enum task_kind kind);

	static std::string compute_color(double hue);

	int compare(const struct task*, const struct task*);

	int process();

	void print();
};

}  // namespace pool
