class ThreadPool
{
public:
    ~ThreadPool();

    void init(UINT threadCount);
    void init(UINT threadCount, const Vector<ThreadLocalStorage*> &threadLocalStorage);
    void runTasks(TaskList<WorkerThreadTask*> &tasks, bool useConsole = true);

private:
    Vector<WorkerThread> m_threads;
};
