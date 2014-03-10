
#ifndef CORE_MULTITHREADING_TASKLIST_H_
#define CORE_MULTITHREADING_TASKLIST_H_

namespace ml
{

template <class T> class TaskList
{
public:
    void insert(const T &task)
    {
        m_mutex.acquire();
        m_tasks.pushBack(task);
        m_mutex.release();
    }

    bool done()
    {
        m_mutex.acquire();
        bool result = (m_tasks.size() == 0);
        m_mutex.release();
        return result;
    }

    UINT64 tasksLeft()
    {
        m_mutex.acquire();
        UINT64 result = m_tasks.size();
        m_mutex.release();
        return result;
    }

    bool getNextTask(T &nextTask)
    {
        m_mutex.acquire();
        if(m_tasks.size() == 0)
        {
            m_mutex.release();
            return false;
        }

        nextTask = m_tasks.back();
        m_tasks.pop_back();
        m_mutex.release();
        return true;
    }

private:
    Mutex m_mutex;
    std::vector<T> m_tasks;
};

}  // namespace ml

#endif  // CORE_MULTITHREADING_TASKLIST_H_
