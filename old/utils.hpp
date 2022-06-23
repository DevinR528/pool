#pragma once

#include <execinfo.h>
#include <string>

namespace pool {

[[noreturn]] void panic_with_trace(std::string_view msg);

}  // namespace pool
