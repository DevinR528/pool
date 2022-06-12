#include "class.hpp"
#include "kind.hpp"

namespace pool {

task::task(task_kind kind) : color(compute_color((double)kind / (double)task_kind::tk_count)) {}

}  // namespace pool
