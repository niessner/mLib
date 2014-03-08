class ThreadPool
{
public:
    ~ThreadPool();

    void init(UINT threadCount);
    void init(UINT threadCount, const std::vector<ThreadLocalStorage*> &threadLocalStorage);
    void runTasks(TaskList<WorkerThreadTask*> &tasks, bool useConsole = true);

private:
    std::vector<WorkerThread> m_threads;
};
