#pragma once

#include <mutex>

class ComplexLock
{
	friend class ComplexGuardLock;
public:

	ComplexLock()
	{
	
	}

	virtual ~ComplexLock()
	{
		
	}

	void Lock()
	{
		m_mutex.lock();
	}

	void UnLock()
	{
		m_mutex.unlock();
	}

	bool TryLock()
	{
		return m_mutex.try_lock();
	}

private:

	std::mutex m_mutex;

};

class ComplexGuardLock
{
public:

	explicit ComplexGuardLock(ComplexLock* lock)
		: m_lock(lock)
	{
		Lock();
	}

	virtual ~ComplexGuardLock()
	{
		UnLock();
	}

	void Lock()
	{
		m_lock->Lock();
	}

	void UnLock()
	{
		m_lock->UnLock();
	}

private:

	ComplexLock* m_lock;

};