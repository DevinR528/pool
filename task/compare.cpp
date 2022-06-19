#include <string>

#include "class.hpp"
#include "kind.hpp"

namespace pool {

int task::compare(const task& a, const task& b) {
	return a.color < b.color
		? -1 : a.color == b.color
		? 0 : 1;
}

}  // namespace pool
