


int nain(int argc, char const* argv[])
{
	int error = 0;
	
	struct cmdln* flags = new cmdln(argc, argv);
	
	struct pqueue* pqueue = new pqueue();
	
	struct aio_scheduler* aio = new aio_scheduler(pqueue);
		// schedule_write(fd, buffer, size, task):
			// submits write(fd, buffer, size) request
			// on the event of a release:
				// submit task into pqueue
	
	struct dep_scheduler* dep = new dep_scheduler(pqueue);
		// dict: dependency -> tasks[]
		
		// schedule(dependencies[], task);
			// data-structure entry
		
		// release(dependency);
			// might add tasks to pqueue
	
	struct scope* scope = new scope();
	
	struct tpool* tpool = new tpool(pqueue, flags->number_of_threads);
	
	pqueue->submit(new parse_task(flags->input_path));
	
	tpool->join();
	
	scope->printout();
	
	return error;
}


int main(int argc, char const* argv[]) {
	#ifdef DEBUGGING
	signal(SIGSEGV, segv_backtrace);
	#endif
	
	try {
		return nain(argc, argv);
	} catch (const std::bad_alloc& e) {
		std::cout << "Allocation failed: " << e.what() << '\n';
	}
}











