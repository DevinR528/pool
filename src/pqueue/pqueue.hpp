#pragma once

#include <atomic>
#include <concepts>
#include <coroutine>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

namespace pool {

template<typename T, typename Compare = std::less<T>> class pqueue {
	template<bool Can, class Empty = void> using enable_if_t =
		typename std::enable_if<Can, Empty>::type;

	std::atomic<T*> head;
	std::atomic<size_t> data_len;
	std::atomic<size_t> data_cap;

	std::vector<std::unique_ptr<T>> data{};

	Compare cmp;

  public:
	explicit pqueue(size_t size) : data_len(0), data_cap(size), cmp(Compare()) {}
	explicit pqueue(size_t size, Compare cmp) : data_len(0), data_cap(size), cmp(cmp) {}

	// No copying of this, we can't lose our atomic syncs
	pqueue(pqueue<T, Compare>& queue) = delete;

	explicit pqueue(pqueue<T, Compare>&& queue) {
		auto relaxed = std::memory_order_relaxed;

		this->head.store(queue.head.load(relaxed), relaxed);
		this->data_len.store(queue.data_len.load(relaxed), relaxed);
		std::swap(this->data, queue.data);
		std::swap(this->cmp, queue.cmp);
	}

	template<class... Args, enable_if_t<std::is_constructible<T, Args&&...>::value>* = nullptr>
	bool push(Args&&... item) {
		size_t len = this->data_len.load();
		if (this->data_cap.load() <= len) {
			// TODO: lock and resize???
		}

		this->data.push_back(std::make_unique<T>(std::forward<Args>(item)...));
		auto old = this->data_len.fetch_add(1);
		std::cout << old << "\n";

		if (old == 0) {
			this->head = this->data[0].get();
		}
		std::cout << this->data_len.load() << "\n";
		std::cout << this->data_cap.load() << "\n";
		return true;
	}

	std::optional<std::unique_ptr<T>> pop() {
		if (this->data_len.load() == 0) { return {}; }

		auto res = this->head.load();
		// While the head of the list is not the current data[0] we can't move our res out.
		//
		// if this returns false that means pop() was called from another thread while between here
		// and the while
		while (!this->head.compare_exchange_weak(
			res,
			this->data[1].get(),
			std::memory_order_release,
			std::memory_order_relaxed)) {};

		this->data_len.fetch_sub(1);

		return std::make_unique<T>(*res);
	}

	bool heap_sort();
};

}  // namespace pool
