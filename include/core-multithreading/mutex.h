
class Mutex
{
public:
    bool tryAcquire()
    {
        return m_mutex.try_lock();
    }
    void acquire()
    {
        m_mutex.lock();
    }
    void release()
    {
        m_mutex.unlock();
    }

private:
    std::mutex m_mutex;
};
