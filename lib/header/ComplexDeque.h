#pragma once

#include <initializer_list>


namespace ComplexLibrary
{
	template <typename T>
	class ComplexDeque
	{
	public:

		ComplexDeque(int maxsize = 10)
			: m_ptr(nullptr)
			, m_maxSize(0)
			, m_size(0)
			, m_front_index(0)
			, m_back_index(0)
		{
			m_maxSize = maxsize;
			m_ptr = new T[maxsize];
		}

		ComplexDeque(ComplexDeque& other)
			: m_ptr(nullptr)
			, m_maxSize(0)
			, m_size(0)
			, m_front_index(0)
			, m_back_index(0)
		{
			m_maxSize = other.m_maxSize;
			m_size = other.m_size;
			m_front_index = other.m_front_index;
			m_back_index = other.m_back_index;
			m_ptr = new T[m_maxSize];

			for (int i = 0; i < m_maxSize; i++)
			{
				m_ptr[i] = other.m_ptr[i];
			}
		}

		ComplexDeque(std::initializer_list<T> list)
			: m_ptr(nullptr)
			, m_maxSize(0)
			, m_size(0)
			, m_front_index(0)
			, m_back_index(0)
		{
			int size = list.size();
			m_maxSize = size;

			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_ptr = new T[m_maxSize];

			auto iter = list.begin();

			while (iter != list.end())
			{
				push_back(*iter);
				iter++;
			}
		}

		~ComplexDeque()
		{
			delete[] m_ptr;
			m_ptr = nullptr;
		}

		void push_back(T value)
		{
			if (m_ptr == nullptr)
				throw "Index Out of Bound";
			if (full())
				throw "Index Out of Bound";

			m_back_index = (m_back_index + 1) % m_maxSize;
			m_ptr[m_back_index] = value;
			m_size++;
		}

		void push_front(T value)
		{
			if (m_ptr == nullptr)
				throw "Index Out of Bound";
			if (full())
				throw "Index Out of Bound";

			m_ptr[m_front_index] = value;
			m_front_index = (m_front_index - 1 + m_maxSize) % m_maxSize;
			m_size++;
		}

		void pop_front()
		{
			if (m_ptr == nullptr)
				throw "Index Out of Bound";
			if (m_front_index == 0)
				throw "Index Out of Bound";

			m_front_index = (m_front_index + 1) % m_maxSize;
			m_size--;
		}

		void pop_back()
		{
			if (m_ptr == nullptr)
				throw "Index Out of Bound";
			if (m_back_index == 0)
				throw "Index Out of Bound";

			m_back_index = (m_back_index - 1 + m_maxSize) % m_maxSize;
			m_size--;
		}

		T& at(int index)
		{
			if (m_ptr == nullptr)
				throw "Index Out of Bound";
			if (index < 0)
				throw "Index Out of Bound";
			if (index > m_maxSize)
				throw "Index Out of Bound";

			return m_ptr[(m_front_index + index) % m_maxSize];
		}

		T& front()
		{
			if (m_ptr == nullptr)
				throw "Index Out of Bound";
			if (m_front_index == 0)
				throw "Index Out of Bound";

			return m_ptr[(m_front_index + 1) % m_maxSize];
		}

		T& back()
		{
			if (m_ptr == nullptr)
				throw "Index Out of Bound";
			if (m_back_index == 0)
				throw "Index Out of Bound";

			return m_ptr[m_back_index];
		}

		int size() const
		{
			return m_size;
		}

		bool empty()
		{
			return m_size == 0;
		}

		bool full()
		{
			return m_size == m_maxSize;
		}

		void clear()
		{
			m_front_index = 0;
			m_back_index = 0;
			m_size = 0;
		}

		T& operator [] (int index)
		{
			return m_ptr[(m_front_index + index) % m_maxSize];
		}

		ComplexDeque& operator = (ComplexDeque& other)
		{
			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_maxSize = other.m_maxSize;
			m_size = other.m_size;
			m_front_index = other.m_front_index;
			m_back_index = other.m_back_index;
			m_ptr = new T[m_maxSize];

			for (int i = 0; i < m_maxSize; i++)
			{
				m_ptr[i] = other.m_ptr[i];
			}
		}

		ComplexDeque& operator = (std::initializer_list<T> list)
		{
			int size = list.size();
			m_maxSize = size;
			m_size = 0;

			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_ptr = new T[m_maxSize];

			auto iter = list.begin();

			while (iter != list.end())
			{
				push_back(*iter);
				iter++;
			}

			return *this;
		}

	private:

		T* m_ptr;
		int m_maxSize;
		int m_size;

		int m_front_index;
		int m_back_index;

	};
}
