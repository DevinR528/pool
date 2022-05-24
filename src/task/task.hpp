#pragma once

#include "macro.hpp"
#include "pqueue/pqueue.hpp"
#include "utils.hpp"

#include <concepts>
#include <coroutine>
#include <functional>
#include <future>
#include <variant>

namespace pool {

enum task_status { ts_WORKING, ts_SUCCESS, ts_FAILED };

enum task_tag { t_PRIO_CTX, t_PRIO_INT, t_PRIO_INT_INT };
class task {
  private:
	template<typename Item = void> struct _less;
	template<typename Item> struct _less: public std::binary_function<Item, Item, bool> {
		constexpr bool operator()(const Item& a, const Item& b) const { return a < b; }
	};

  public:
	template<typename CoRet> struct promise_type {
		using coro_handle = std::coroutine_handle<promise_type<CoRet>>;

		coro_handle hndl;
		CoRet promise_value;

		promise_type() {}
		promise_type(coro_handle hndl) : hndl(hndl) {}

		promise_type<CoRet> get_return_object() {
			return promise_type<CoRet>(coro_handle::from_promise(*this));
		}
		std::suspend_never initial_suspend() noexcept { return {}; }
		// We MUST return `std::suspend_always` here so that we can use the `co_return` value,
		// otherwise we segfault because the coroutine has be destroyed (I think?)
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_value(CoRet res) { this->promise_value = res; }
		void unhandled_exception() { panic_with_trace("error: task crashed"); }

		operator std::coroutine_handle<promise_type<CoRet>>() const { return this->hndl; }
	};

	using less = _less<std::unique_ptr<task>>;
	using prio_ctx_ptr = std::shared_ptr<pqueue<task, less>>;

  private:
	int32_t prio;
	bool sentinal_end = false;
	task_tag tag;
	// clang-format off
	std::variant<
		std::function<promise_type<bool>(prio_ctx_ptr)>,
		std::function<promise_type<bool>(prio_ctx_ptr, int)>,
		std::function<promise_type<bool>(prio_ctx_ptr, int, int)>
	> variant_func;
	// clang-format on

  public:
	// No such thing as default task.
	//
	// The `prio_queue` points to each task so we don't worry about having to default the backing
	// storage on `prio_queue` initialization.
	task() = delete;

	explicit task(bool end) : sentinal_end(end) {}

	explicit task(int32_t prio, std::function<promise_type<bool>(prio_ctx_ptr)> func) :
		prio(prio),
		tag(t_PRIO_CTX),
		variant_func(func) {}

	explicit task(int32_t prio, std::function<promise_type<bool>(prio_ctx_ptr, int)> func) :
		prio(prio),
		tag(t_PRIO_INT),
		variant_func(func) {}

	constexpr int32_t priority() const& { return this->prio; }
	constexpr bool is_done() const& { return this->sentinal_end; }

	promise_type<bool> call(prio_ctx_ptr queue_ptr) {
		std::cout << "tag: " << this->tag << "\n";
		switch (this->tag) {
			case t_PRIO_CTX:
				return std::get<0>(this->variant_func)(queue_ptr);
			default:
				panic_with_trace("unreachable case in `task::call`");
		}
	}

	using task_ptr = std::unique_ptr<task>;
	friend bool operator<(const task_ptr& a, const task_ptr& b) {
		return a->priority() < b->priority();
	}
	friend bool operator>(const task_ptr& a, const task_ptr& b) { return b < a; }
	friend bool operator<=(const task_ptr& a, const task_ptr& b) { return !(a > b); }
	friend bool operator>=(const task_ptr& a, const task_ptr& b) { return !(a < b); }
};

}  // namespace pool
