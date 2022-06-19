#include <string>

#include "class.hpp"
#include "../kind.hpp"

namespace pool {

parse_task::parse_task(std::string_view path) : task(task_kind::tk_parse), path(path) {}

}  // namespace pool
