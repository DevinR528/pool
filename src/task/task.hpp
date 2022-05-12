#pragma once

#include "../pqueue/pqueue.hpp"
#include "macro.hpp"

#include <concepts>
#include <coroutine>
#include <functional>
#include <liburing.h>
#include <variant>

namespace pool {
enum task_tag { t_PRIO_CTX, t_PRIO_INT, t_PRIO_INT_INT };
class task {
  private:
	template<typename Item = void> struct _less;
	template<typename Item> struct _less: public std::binary_function<Item, Item, bool> {
		constexpr bool operator()(const Item& a, const Item& b) const { return a < b; }
	};

  public:
	using less = _less<task>;
	using prio_ctx_ptr = std::shared_ptr<pqueue<task, less>>;

  private:
	int32_t prio;

	task_tag tag;
	// clang-format off
	std::variant<
		std::function<bool(prio_ctx_ptr)>,
		std::function<bool(prio_ctx_ptr, int)>,
		std::function<bool(prio_ctx_ptr, int, int)>
	> variant_func;
	// clang-format on

  public:
	// No such thing as default task.
	//
	// The `prio_queue` points to each task so we don't worry about having to default the backing
	// storage on `prio_queue` initialization.
	task() = delete;

	task(int32_t prio, std::function<bool(prio_ctx_ptr)> func) :
		prio(prio),
		tag(t_PRIO_CTX),
		variant_func(func) {}

	task(int32_t prio, std::function<bool(prio_ctx_ptr, int)> func) :
		prio(prio),
		tag(t_PRIO_INT),
		variant_func(func) {}

	bool call(prio_ctx_ptr queue_ptr) {
		switch (this->tag) {
			case t_PRIO_CTX:
				return std::get<0>(this->variant_func)(queue_ptr);
			default:
				return false;
		}
	}

	friend constexpr bool operator<(const task& a, const task& b) { return a.prio < b.prio; }
	friend constexpr bool operator>(const task& a, const task& b) { return b < a; }
	friend constexpr bool operator<=(const task& a, const task& b) { return !(a > b); }
	friend constexpr bool operator>=(const task& a, const task& b) { return !(a < b); }
};

}  // namespace pool
