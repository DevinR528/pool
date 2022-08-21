#include <csignal>
#include <iostream>

#include "misc/segv_backtrace.hpp"
#include "task/class.hpp"
#include "task/parse/class.hpp"
#include "task/tyck/class.hpp"
#include "tpool/class.hpp"
// #include ""
// #include ""

int nain(int argc, char const* argv[]) {
	int error = 0;

	std::unique_ptr<pool::parse_task> pt =
		std::make_unique<pool::parse_task>(pool::parse_task(argv[1]));

	std::unique_ptr<pool::tyck_task> tt = std::make_unique<pool::tyck_task>(pool::tyck_task());

	std::vector<std::unique_ptr<pool::task>> pqueue;

	pqueue.push_back(std::move(pt));
	pqueue.push_back(std::move(tt));

	std::sort(
		pqueue.begin(),
		pqueue.end(),
		[](const std::unique_ptr<pool::task>& a, const std::unique_ptr<pool::task>& b) {
			std::cout << a->kind << " " << b->kind << std::endl;
			// Sort it backwards since the vector only has pop_back
			return a->compare(*b.get()) > 0;
		});

	pool::thread_pool tpool = pool::thread_pool(std::move(pqueue), 2);
	tpool.join();

	// 	struct cmdln* flags = cmdln(argc, argv);

	// 	struct pqueue* pqueue = new pqueue();

	// 	struct aio_scheduler* aio = new aio_scheduler(pqueue);
	// 		// schedule_write(fd, buffer, size, task):
	// 			// submits write(fd, buffer, size) request
	// 			// on the event of a release:
	// 				// submit task into pqueue

	// 	struct dep_scheduler* dep = new dep_scheduler(pqueue);
	// 		// dict: dependency -> tasks[]

	// 		// schedule(dependencies[], task);
	// 			// data-structure entry

	// 		// release(dependency);
	// 			// might add tasks to pqueue

	// 	struct scope* scope = new scope();

	// 	struct tpool* tpool = new tpool(pqueue, flags->number_of_threads);

	// 	pqueue->submit(new parse_task(flags->input_path));

	// 	tpool->join();

	// 	scope->printout();

	return argc > 0 ? error : 0;
}

int main(int argc, char const* argv[]) {
#ifdef DEBUGGING
	signal(SIGSEGV, pool::segv_backtrace);
#endif

	try {
		return nain(argc, argv);
	} catch (const std::bad_alloc& e) { std::cout << "Allocation failed: " << e.what() << '\n'; }

	return 0;
}
