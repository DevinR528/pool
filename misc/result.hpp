
// vim: set tabstop=4 expandtab smarttab

#pragma once

#include <assert.h>
#include <functional>
#include <iostream>
#include <type_traits>
#include <variant>

namespace pool {

template<class OkVal, class ErrVal> struct result {
  private:
	template<bool Can, class Empty = void> using enable_if_t =
		typename std::enable_if<Can, Empty>::type;

	bool m_has_value;
	std::variant<OkVal, ErrVal> m_value;

  public:
	// Remove the default constructor, no null `result`s cus what does that mean.
	result() {}

	// Copy constructor, memcpy the value in this `result` could be error or success.
	result(const result& res) : m_has_value(res.m_has_value) {
		memcpy(m_value, res.m_value, sizeof(std::variant<OkVal, ErrVal>));
	}

	// Move constructor, move a temporary (rvalue) into the new `result`
	result(result&& res) : m_has_value(res.m_has_value), m_value(res.m_value) {}

	result& operator=(const result& res) {
		m_has_value = res.has_value();
		m_value = std::move(res.m_value);
		return *this;
	}

	result& operator=(result&& res) {
		m_has_value = res.m_has_value;
		m_value = std::move(res.m_value);
		return *this;
	}

	template<class Arg, enable_if_t<std::is_constructible<OkVal, Arg>::value>* = nullptr>
	static result<OkVal, ErrVal> ok(Arg arg) {
		return result(arg);
	}
	template<class Arg, enable_if_t<std::is_constructible<ErrVal, Arg&&>::value>* = nullptr>
	result(Arg&& arg) :
		m_has_value(false),
		m_value(std::variant<OkVal, ErrVal>(std::forward<Arg>(arg))) {}

	template<class... Args, enable_if_t<std::is_constructible<OkVal, Args&&...>::value>* = nullptr>
	result(Args&&... args) :
		m_has_value(true),
		m_value(std::variant<OkVal, ErrVal>(std::forward<Args>(args)...)) {}
	template<class... Args, enable_if_t<std::is_constructible<ErrVal, Args&&...>::value>* = nullptr>
	result(Args&&... args) :
		m_has_value(false),
		m_value(std::variant<OkVal, ErrVal>(std::forward<Args>(args)...)) {}

	template<
		class U,
		class... Args,
		enable_if_t<std::is_constructible<OkVal, std::initializer_list<U>&, Args&&...>::value>* =
			nullptr>
	result(std::initializer_list<U> il, Args&&... args) :
		m_has_value(true),
		m_value(std::variant<OkVal, ErrVal>(il, std::forward<Args>(args)...)) {}

	template<
		class U,
		class... Args,
		enable_if_t<std::is_constructible<ErrVal, std::initializer_list<U>&, Args&&...>::value>* =
			nullptr>
	result(std::initializer_list<U> il, Args&&... args) :
		m_has_value(false),
		m_value(std::variant<OkVal, ErrVal>(il, std::forward<Args>(args)...)) {}

	constexpr bool has_value() const noexcept { return this->m_has_value; }
	constexpr explicit operator bool() const noexcept { return this->m_has_value; }

	constexpr OkVal& value() & { return std::get<0>(this->m_value); }
	constexpr ErrVal& error() & { return std::get<1>(this->m_value); }

	// template<class Fn, class Ret = decltype(std::mem_fn(std::declval<Fn>()))>
	// constexpr Ret and_then(Fn&& func) & {
	// 	return this->has_value() ? std::invoke(std::forward<Fn>(func), *this) :
	// Ret(this->error());
	// }
	template<typename T, typename E>
	friend std::ostream& operator<<(std::ostream& os, const result<T, E>& res);
};

template<typename T, typename E>
std::ostream& operator<<(std::ostream& os, const result<T, E>& res) {
	os << "hey" << std::endl;
	return os;
}
}  // namespace pool
