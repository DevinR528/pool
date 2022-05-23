#pragma once

#include <string>
#include <execinfo.h>

namespace pool {

[[noreturn]] void panic_with_trace(std::string_view msg);

}  // namespace pool
