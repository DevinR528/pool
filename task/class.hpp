
class task
{
	public:
	
	task() = delete;
	task(enum task_kind kind);
	
	int compare(const struct task*, const struct task*);
	
	int process();
	
	void print();
};

