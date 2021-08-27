#pragma once

#include <mutex>
#include <thread>
#include <chrono>

namespace ComplexLibrary
{
	class ComplexLock
	{
		friend class ComplexCondition;
	public:

		ComplexLock(ComplexLock* lock = nullptr)
		{
			AttachCriticalSection(lock);
		}

		virtual ~ComplexLock()
		{
			DetachCriticalSection();
		}

		void AttachCriticalSection(ComplexLock* lock = nullptr)
		{
			if (lock == nullptr)
				m_uniquelock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
			else
				m_uniquelock = std::unique_lock<std::mutex>(lock->m_mutex, std::defer_lock);
		}

		void DetachCriticalSection()
		{
			m_uniquelock.release();
		}

		void Lock()
		{
			try
			{
				m_uniquelock.lock();
			}
			catch (std::system_error e)
			{
				// ������ε� lock�� ���� ������ �� ���� blocking ��Ŵ
				Lock();
			}
		}

		void UnLock()
		{
			try
			{
				m_uniquelock.unlock();
			}
			catch (std::system_error e)
			{
				// �̹� unlock �����̰ų� lock�� �ʱ�ȭ�ȵ� ���� �׳� ���Ͻ�Ŵ
				return;
			}
		}

		bool TryLock()
		{
			bool btry = false;
			try
			{
				btry = m_uniquelock.try_lock();
			}
			catch (std::system_error e)
			{
				// ����� �߻�
				btry = false;
			}
			return btry;
		}

	private:

		std::mutex m_mutex;
		std::unique_lock<std::mutex> m_uniquelock;
	};

	class ComplexScopedLock
	{
		friend class ComplexCondition;
	public:

		explicit ComplexScopedLock(ComplexLock* lock)
		{
			m_lock.AttachCriticalSection(lock);
			m_lock.Lock();
		}

		virtual ~ComplexScopedLock()
		{
			m_lock.UnLock();
		}

		void Lock()
		{
			m_lock.Lock();
		}

		void UnLock()
		{
			m_lock.UnLock();
		}

		ComplexLock& GetLock()
		{
			return m_lock;
		}

	private:

		ComplexLock m_lock;
	};
}