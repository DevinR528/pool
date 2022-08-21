#include <string>

#include "class.hpp"
#include "kind.hpp"

namespace pool {

int task::compare(const task& b) const& {
	return this->kind < b.kind ? -1 : this->kind == b.kind ? 0 : 1;
}

}  // namespace pool
