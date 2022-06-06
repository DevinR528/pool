
def thread(pqueue):
	
	while (1):
		task = pqueue->pop();
		
		co_await(task);
		
		if (task->is_done())
			delete (task);
	
