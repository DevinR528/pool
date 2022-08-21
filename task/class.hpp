#pragma once

#include <concepts>
#include <coroutine>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../enums/error.hpp"
#include "../misc/panic_with_trace.hpp"
#include "../misc/result.hpp"
#include "kind.hpp"

namespace pool {
class task {
  public:
	// Coroutine magic (must be defined within the class...I think)
	template<typename CoRet, typename CoYield=char[4]> struct promise_type {
		using coro_handle = std::coroutine_handle<promise_type<CoRet, CoYield>>;

		coro_handle m_hndl;
		CoRet m_promise_value;
		CoYield m_yield_val;

		promise_type() {}
		promise_type(coro_handle hndl) : m_hndl(hndl) {}

		promise_type<CoRet, CoYield> get_return_object() {
			return promise_type<CoRet, CoYield>(coro_handle::from_promise(*this));
		}
		std::suspend_always initial_suspend() noexcept { return {}; }
		// We MUST return `std::suspend_always` here so that we can use the `co_return` value,
		// otherwise we segfault because the coroutine has been destroyed (I think?)
		std::suspend_always final_suspend() noexcept { return {}; }

		constexpr bool await_ready() const noexcept {
			std::cout << "in ready" << std::endl;
			return true;
		}

		CoRet await_resume() const noexcept {
			std::cout << "in resume" << std::endl;
			return m_promise_value;
		}

		void await_suspend(coro_handle& h) const noexcept {
			std::cout << "in suspend" << std::endl;
			// m_hndl.resume();
		}

		template<std::convertible_to<CoYield> From>
		std::suspend_always yield_value(From&& from) {
			this->m_yield_val = std::forward<From>(from);
			return {};
		}

		void return_value(CoRet res) { this->m_promise_value = res; }
		void unhandled_exception() { panic_with_trace("error: task crashed"); }

		operator std::coroutine_handle<promise_type<CoRet, CoYield>>() const { return this->m_hndl; }

		struct iterator {
			bool operator!=(const iterator& rhs) { return not m_h_ptr->done(); }
			iterator& operator++() {
				m_h_ptr->resume();
				return *this;
			}
			std::string operator*() { return m_h_ptr->promise().m_yield_val; }
			std::coroutine_handle<promise_type<CoRet, CoYield>>* m_h_ptr;
		};
		iterator begin() { return iterator{&this->m_hndl}; }
		iterator end() { return iterator{nullptr}; }
	};

	task_kind kind;
	std::string color;

	task();
	task(task_kind kind) noexcept;
	task(const task&) = delete;
	task(task&&) noexcept;

	task& operator=(task&& other) {
		this->kind = std::move(other.kind);
		this->color = std::move(other.color);
		return *this;
	}

	static std::string compute_color(double hue);

	int compare(const task&) const&;

	void print();

	virtual promise_type<result<bool, pool_error>> process(std::vector<std::unique_ptr<task>>&) = 0;

	virtual ~task() {
		this->color.~basic_string();
	}
};

}  // namespace pool
