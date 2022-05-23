#pragma once

#include "utils.hpp"

#include <algorithm>
#include <atomic>
#include <concepts>
#include <coroutine>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>

namespace pool {

// TODO: actually implement a good lock-free concurrent queue
template<typename T, typename Compare = std::less<T>> class pqueue {
	template<bool Can, class Empty = void> using enable_if_t =
		typename std::enable_if<Can, Empty>::type;

	std::mutex lock;
	std::deque<std::unique_ptr<T>> data{};
	Compare cmp;

  public:
	pqueue() : cmp(Compare()) {}
	explicit pqueue(Compare cmp) : cmp(cmp) { }

	// No copying of this, we can't lose our atomic syncs
	pqueue(pqueue<T, Compare>& queue) = delete;

	explicit pqueue(pqueue<T, Compare>&& queue) {
		// std::swap(this->lock, queue.lock);
		std::swap(this->data, queue.data);
		std::swap(this->cmp, queue.cmp);
	}

	template<class... Args, enable_if_t<std::is_constructible<T, Args&&...>::value>* = nullptr>
	void push(Args&&... item) {
		this->data.emplace_front(std::make_unique<T>(std::forward<Args>(item)...));

		if (this->data.size() > 1) {
			this->lock.lock();
			std::sort(this->data.begin(), this->data.end(), this->cmp);
			this->lock.unlock();
		}

		// std::cout << "[";
		// for (auto&& el : this->data) {
		// 	std::cout << el->priority() << ",";
		// }
		// std::cout << "]\n";
	}

	std::optional<std::unique_ptr<T>> pop() {
		if (this->data.size() == 0) { return {}; }

		this->lock.lock();

		auto item = std::move(this->data.front());
		this->data.pop_front();

		this->lock.unlock();

		return item;
	}
};

}  // namespace pool
