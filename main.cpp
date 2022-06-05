


int nain(int argc, char const* argv[])
{
	int error = 0;
	
	struct cmdln* flags = new cmdln(argc, argv);
	
	struct pqueue* pqueue = new pqueue();
	
	struct tpool* tpool = new tpool(pqueue, flags->number_of_threads);
	
	
	
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




