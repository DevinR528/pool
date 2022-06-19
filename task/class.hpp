#pragma once

#include <string>
#include <concepts>
#include <coroutine>

#include "kind.hpp"
#include "../misc/panic_with_trace.hpp"

namespace pool {
class task {
  public:
	task_kind kind;
	std::string color;

	task();
	task(enum task_kind kind);

	static std::string compute_color(double hue);

	int compare(const task&, const task&);

	void print();

	virtual int process() = 0;

	virtual ~task() {}

	// Coroutine magic (must be defined within the class...I think)
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
		// otherwise we segfault because the coroutine has been destroyed (I think?)
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_value(CoRet res) { this->promise_value = res; }
		void unhandled_exception() { panic_with_trace("error: task crashed"); }

		operator std::coroutine_handle<promise_type<CoRet>>() const { return this->hndl; }
	};
};

}  // namespace pool
