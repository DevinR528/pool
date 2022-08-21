#include <concepts>
#include <memory>
#include <vector>

#include "../../enums/error.hpp"
#include "class.hpp"

namespace pool {

/// Read a file asyncroniously yielding chunks as we go.
task::promise_type<pool_error, std::string>
aio_file_read(std::vector<std::unique_ptr<task>>& que, std::string_view path);

}  // namespace pool
