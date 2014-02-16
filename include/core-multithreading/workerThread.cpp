#include "Main.h"

/*void WorkerThreadEntry( LPVOID context )
{
	WorkerThread *thread = (WorkerThread *)context;
	thread->beginTaskLoop();
}*/

void WorkerThread::init(UINT threadIndex, ThreadLocalStorage *storage)
{
	m_threadIndex = threadIndex;
	m_storage = storage;
}

void WorkerThread::processTasks(TaskList<WorkerThreadTask*> &tasks)
{
	m_done = false;
	m_tasks = &tasks;
	m_thread = std::thread(workerThreadEntry, this);
	_thread.BeginCdecl(WorkerThreadEntry, this);
}

void WorkerThread::workerThreadEntry( WorkerThread *context )
{
	context->enterThreadTaskLoop();
}

void WorkerThread::enterThreadTaskLoop()
{
	WorkerThreadTask* curTask;
	while(_tasks->GetNextTask(curTask))
	{
		curTask->Run(_threadIndex, _storage);
		delete curTask;
	}
	_done = true;
}
