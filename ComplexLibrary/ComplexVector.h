#ifndef bruno_include_ComplexVector_H 
#define bruno_include_ComplexVector_H 1

template <typename T>
class ComplexVector
{
public:

	ComplexVector(int capacity = 4)
	{
		m_ptr = new T[capacity];
		m_capacity = capacity;
		m_size = 0;
	}

	ComplexVector(const ComplexVector<T>& ptr)
	{
		int nPtr_size = ptr.m_size;
		int nPtr_capacity = ptr.m_capacity;

		if (m_ptr != nullptr)
			delete[] m_ptr;

		m_ptr = new T[ptr.m_capacity];

		for (int i = 0; i < nPtr_capacity; i++)
		{
			m_ptr[i] = ptr.m_ptr[i];
		}

		m_size = nPtr_size;
		m_capacity = nPtr_capacity;

	}

	~ComplexVector()
	{
		if (m_ptr != nullptr)
			delete[] m_ptr;
	}

	void push_back(T value)
	{
		if (m_size + 1 > m_capacity)
		{
			T* tmp = new T[m_capacity * 2];

			for (int i = 0; i < m_size; i++)
			{
				tmp[i] = m_ptr[i];
			}

			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_ptr = tmp;
			m_capacity *= 2;

		}

		m_ptr[m_size] = value;
		m_size++;
	}

	void reserve(int capacity)
	{
		if (capacity < m_capacity)
			return;

		T* tmp = new T[capacity];
		for (int i = 0; i < m_size; i++)
		{
			tmp[i] = m_ptr[i];
		}

		delete[] m_ptr;
		m_ptr = tmp;
		m_capacity = capacity;
	}

	T& at(int idx)
	{
		if (m_size <= 0) throw "Index Out of Bound";
		if ((m_size - 1) < idx) throw "Index Out of Bound";

		return m_ptr[idx];
	}

	T& front() const
	{
		return m_ptr[0];
	}

	T& back() const
	{
		return m_ptr[m_size - 1];
	}

	T* begin() const
	{
		return m_ptr;
	}

	T* end() const
	{
		return m_ptr + m_size;
	}

	void erase(int idx)
	{
		if (m_size <= 0) throw "Index Out of Bound";
		if ((m_size - 1) < idx) throw "Index Out of Bound";

		T* tmp = new T[m_capacity];
		int tmpidx = 0;

		for (int i = 0; i < idx; i++)
		{
			tmp[tmpidx] = m_ptr[i];
			tmpidx++;
		}

		for (int i = idx + 1; i < m_size; i++)
		{
			tmp[tmpidx] = m_ptr[i];
			tmpidx++;
		}

		delete[] m_ptr;
		m_ptr = tmp;
		m_size--;
	}

	void erase(int startidx, int endidx)
	{
		if (m_size <= 0) throw "Index Out of Bound";
		if ((m_size - 1) < startidx) throw "Index Out of Bound";
		if ((m_size - 1) < endidx) throw "Index Out of Bound";
		if (endidx < startidx) throw "Index Out of Bound";

		T* tmp = new T[m_capacity];
		int tmpidx = 0;

		for (int i = 0; i < startidx; i++)
		{
			tmp[tmpidx] = m_ptr[i];
			tmpidx++;
		}

		for (int i = endidx + 1; i < m_size; i++)
		{
			tmp[tmpidx] = m_ptr[i];
			tmpidx++;
		}

		delete[] m_ptr;
		m_ptr = tmp;
		m_size = m_size - (endidx - startidx + 1);
	}

	void pop_back()
	{
		if (m_size <= 0)
		{
			m_size = 0;
		}
		else
		{
			m_size--;
		}
	}

	void clear()
	{
		m_size = 0;
	}

	int size() const
	{
		return m_size;
	}

	int capacity() const
	{
		return m_capacity;
	}

	bool empty() const
	{
		return m_size == 0;
	}

	ComplexVector& operator = (const ComplexVector& other)
	{
		if (this != &other)
		{
			if (m_capacity != other.m_capacity)
			{
				m_capacity = other.m_capacity;
				delete[] m_ptr;

				m_ptr = new T[m_capacity];
			}

			m_size = other.m_size;
			for (int i = 0; i < m_size; i++)
			{
				m_ptr[i] = other.m_ptr[i];
			}
		}
		return *this;
	}

	T& operator [] (int idx)
	{
		return m_ptr[idx];
	}

private:

	T* m_ptr;
	int m_size;
	int m_capacity;


};

#endif /* bruno_include_ComplexVector_H */
