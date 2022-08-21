#pragma once

#include <concepts>
#include <coroutine>
#include <execinfo.h>
#include <iostream>
#include <liburing.h>
#include <memory>

#include "../utils.hpp"

namespace pool {

struct sqe_await {
	io_uring_sqe* sqe;

	struct resume_res {
		std::coroutine_handle<> handle;
		int res;

		void resolve(int res) {
			this->res = res;
			this->handle.resume();
		}
	};

  public:
	sqe_await(io_uring_sqe* sqe) noexcept : sqe(sqe) {}

	auto operator co_await() {
		struct awaiter {
			resume_res resolver;
			io_uring_sqe* sqe;

			awaiter(io_uring_sqe* sqe) : sqe(sqe) {}
			// Is the coroutine ready immediately, before being suspended or anything
			// (shortcircit to avoid cost of suspending and resuming)
			constexpr bool await_ready() const noexcept { return false; }

			// We want to return control to caller and put this in the thread pool
			void await_suspend(std::coroutine_handle<> hndl) noexcept {
				this->resolver.handle = hndl;
				io_uring_sqe_set_data(this->sqe, &this->resolver);
			}

			constexpr int await_resume() const noexcept { return this->resolver.res; }
		};
		return awaiter(this->sqe);
	}
};

class io_ringbuf {
	io_uring ring;
	uint32_t cqe_count = 0;
	bool prob_ops[IORING_OP_LAST] = {};

	sqe_await await_work(io_uring_sqe* sqe, uint8_t iflags) {
		io_uring_sqe_set_flags(sqe, iflags);
		return sqe_await(sqe);
	}

  public:
	io_ringbuf() = delete;
	io_ringbuf(io_ringbuf& ring) = delete;
	io_ringbuf(const io_ringbuf& ring) = delete;

	// io_ringbuf(io_ringbuf&& ring) {} ??

	io_ringbuf(int32_t entries = 64, uint32_t flags = 0, uint32_t wq_fd = 0) {
		io_uring_params params;
		params.flags = flags;
		params.wq_fd = wq_fd;

		int err_no;
		if ((err_no = io_uring_queue_init_params(entries, &this->ring, &params))) {
			panic_with_trace("io_uring init failed", err_no);
		}
	}
	~io_ringbuf() { io_uring_queue_exit(&this->ring); }

	[[nodiscard]] io_uring_sqe* get_io_uring_sqe() {
		io_uring_sqe* sqe = io_uring_get_sqe(&this->ring);
		if (sqe) {
			return sqe;
		} else {
			io_uring_cq_advance(&this->ring, this->cqe_count);
			this->cqe_count = 0;
			io_uring_submit(&this->ring);
			sqe = io_uring_get_sqe(&this->ring);
			if (sqe) { return sqe; }
			panic_with_trace("get_io_uring_sqe failed to get submit queue", ENOMEM);
		}
	}

	sqe_await read(int fd, void* buf, uint32_t nbytes, off_t offset, uint8_t iflags = 0) {
		io_uring_sqe* sqe = this->get_io_uring_sqe();
		io_uring_prep_read(sqe, fd, buf, nbytes, offset);
		return this->await_work(sqe, iflags);
	}
};

}  // namespace pool
