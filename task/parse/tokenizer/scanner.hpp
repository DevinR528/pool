// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "../../class.hpp"
#include "../../../misc/result.hpp"

namespace pool {
template<typename T> using promise = task::promise_type<T>;

struct scanner {
	std::optional<char> curr;
	std::string curr_chunk;
	std::string collected;

	scanner() {}
	promise<result<char, std::string>> next_char();
};

}  // namespace pool
