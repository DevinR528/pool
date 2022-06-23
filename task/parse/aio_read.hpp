#include <concepts>
#include <memory>
#include <vector>

#include "../../enums/error.hpp"
#include "class.hpp"

namespace pool {

namespace aio {
	// template<typename CoYield> struct promise_type {
	// 	using coro_handle = std::coroutine_handle<promise_type<CoYield>>;

	// 	coro_handle hndl;
	// 	pool_error ret_val;
	// 	CoYield yield_val;

	// 	promise_type() {}
	// 	promise_type(coro_handle hndl) : hndl(hndl) {}

	// 	inline promise_type get_return_object() {
	// 		return promise_type(coro_handle::from_promise(*this));
	// 	}



	// 	std::suspend_never initial_suspend() noexcept { return {}; }
	// 	// We MUST return `std::suspend_always` here so that we can use the `co_return` value,
	// 	// otherwise we segfault because the coroutine has been destroyed (I think?)
	// 	std::suspend_always final_suspend() noexcept { return {}; }



	// 	void return_value(pool_error res) { this->ret_val = res; }

	// 	void unhandled_exception() { panic_with_trace("error: task crashed"); }

	// 	operator std::coroutine_handle<promise_type<CoYield>>() const { return this->hndl; }
	// };

}  // namespace aio

/// Read a file asyncroniously yielding chunks as we go.
task::promise_type<pool_error>
aio_file_read(std::vector<std::unique_ptr<task>>& que, std::string_view path);

}  // namespace pool
