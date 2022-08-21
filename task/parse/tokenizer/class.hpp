
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "../../../enums/error.hpp"
#include "../../../misc/result.hpp"
#include "../../class.hpp"
#include "./token.hpp"

namespace pool {
template<typename T> using promise = task::promise_type<T>;

// The lexer/tokenizer that turns input text into a `vector<token>`.
class tokenizer {
  private:
	std::string_view m_str_slice;
	const std::string_view m_str_slice_orig;
	std::vector<token> m_tkns{};

	std::optional<char> peek_nth(size_t) noexcept;
	std::optional<char> eat() noexcept;
	std::optional<size_t> eat_while(std::function<bool(char)>) noexcept;
	std::optional<size_t> eat_decimal_digits() noexcept;
	std::optional<size_t> eat_hex_digits() noexcept;
	std::optional<size_t> eat_float_expo() noexcept;

	result<token, std::string> single_line_comment() noexcept;
	result<token, std::string> multi_line_comment() noexcept;
	result<token, std::string> whitespace() noexcept;
	result<token, std::string> identifier() noexcept;
	result<token, std::string> numeric_lit() noexcept;
	result<token, std::string> byte_char() noexcept;
	result<token, std::string> byte_str() noexcept;
	result<token, std::string> raw_str() noexcept;
	result<token, std::string> char_lit() noexcept;
	result<token, std::string> string_lit() noexcept;

	result<token, std::string> next_tkn() noexcept;

  public:
	tokenizer(std::string_view str) : m_str_slice{str}, m_str_slice_orig{str} {}
	~tokenizer() {}

	const std::string_view content() noexcept { return this->m_str_slice_orig; };
	result<std::vector<token>, std::string> lex_input() noexcept;

#ifdef DEBUGGING
	const std::vector<token>& tokens() const& { return this->m_tkns; }
#endif
};

}  // namespace pool
