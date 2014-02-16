
//
// abstract base class for thread local storage
//
class ThreadLocalStorage
{
public:

};

//
// abstract base class for thread tasks
//
class WorkerThreadTask
{
public:
	virtual ~WorkerThreadTask();
    virtual void run(UINT threadIndex, ThreadLocalStorage *threadLocalStorage) = 0;
};

class WorkerThread
{
public:
    void init(UINT threadIndex, ThreadLocalStorage *storage);
    void processTasks(TaskList<WorkerThreadTask*> &tasks);

    bool done()
    {
        return m_done;
    }

private:
	static void workerThreadEntry( WorkerThread *context );
	void enterThreadTaskLoop();

    bool m_done;
    std::thread m_thread;

	UINT m_threadIndex;
    ThreadLocalStorage *m_storage;

    TaskList<WorkerThreadTask*> *m_tasks;
};
