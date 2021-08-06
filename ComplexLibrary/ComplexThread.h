#pragma once

#include <thread>
#include "ComplexLock.h"

class ComplexThread
{
public:

	enum ThreadStatus
	{
		THREAD_STOP = 0,
		THREAD_RUNNING
	};

	ComplexThread()
	{
		m_threadStatus = THREAD_STOP;
	}

	virtual ~ComplexThread()
	{
		Stop();
	}

	bool Start()
	{
		ComplexGuardLock lock(&m_lock);

		if (m_threadStatus == THREAD_RUNNING)
			return false;

		m_thread = std::thread(MainThread, this);
		m_threadStatus = THREAD_RUNNING;

		return true;
	}

	bool Stop()
	{
		ComplexGuardLock lock(&m_lock);

		if (m_threadStatus == THREAD_STOP)
			return false;

		m_threadStatus = THREAD_STOP;
	}

	void Join()
	{
		ComplexGuardLock lock(&m_lock);

		if (m_thread.joinable())
		{
			m_thread.join();
			m_threadStatus = THREAD_STOP;
		}
	}

	bool IsRunning()
	{
		ComplexGuardLock lock(&m_lock);

		return m_threadStatus == THREAD_RUNNING;
	}

	virtual void Run() = 0;

private:

	ThreadStatus m_threadStatus;
	std::thread m_thread;
	ComplexLock m_lock;

};

unsigned __stdcall MainThread(void* param)
{
	ComplexThread* pThis = (ComplexThread*)param;
	pThis->Run();
}