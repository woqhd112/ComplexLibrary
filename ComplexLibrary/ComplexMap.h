#ifndef Koh_include_ComplexMap_H  
#define Koh_include_ComplexMap_H 1

#include "ComplexVector.h"
#include "ComplexLinkedList.h"

#define USE_LINKEDLIST	0

template <typename T, typename N>
struct ComplexPair
{
	T key;
	N value;

	N operator = (N value)
	{
		this->value = value;
		return this->value;
	}
};


template <typename T, typename N>
class ComplexMap
{
private:


	ComplexVector<T> m_keys;
	ComplexLinkedList<ComplexPair<T, N>> m_pairs;

	int m_capacity;
	int m_size;

public:

	typedef ComplexIterator<ComplexPair<T, N>> iterator;

	ComplexMap(int capacity = 4)
	{
		m_keys.reserve(capacity);
		m_capacity = capacity;
		m_size = 0;
	}

	ComplexMap(const ComplexMap<T, N>& ptr)
	{
		int nPtr_size = ptr.m_size;
		int nPtr_capacity = ptr.m_capacity;

		if (m_pairs != nullptr)
			m_pairs.clear();

		if (m_keys != nullptr)
		{
			m_keys.erase(0, m_keys.size() - 1);
			m_keys.clear();
		}

		m_keys.reserve(nPtr_capacity);

		for (int i = 0; i < nPtr_size; i++)
		{
			m_pairs.push_tail(ptr.m_pairs.at(i));
			m_keys.push_back(ptr[i].key);
		}

		m_size = nPtr_size;
		m_capacity = nPtr_capacity;
	}

	~ComplexMap()
	{
		m_pairs.clear();
	}

	void insert(ComplexPair<T, N> pair)
	{
		T key = pair.key;

		Duplicate(key);

		if (m_size + 1 > m_capacity)
		{
			m_keys.reserve(m_capacity * 2);
			m_capacity *= 2;

		}
		m_keys.push_back(key);
		m_pairs.push_tail(pair);
		m_size++;
	}

	iterator find(T key)
	{
		if (m_size <= 0) throw "Empty Map";

		int i = 0;
		for (i = 0; i < m_size; i++)
		{
			if (m_keys.at(i) == key)
			{
				break;
			}
		}

		return iterator(m_pairs.get_at(i));
	}

	iterator begin()
	{
		return iterator(m_pairs.get_head());
	}

	iterator end()
	{
		return iterator(nullptr);
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
		return (m_size == 0);
	}

	void clear()
	{
		m_size = 0;
		m_keys.clear();
		m_pairs.clear();
	}

	ComplexMap<T, N>& operator = (const ComplexMap<T, N>& other)
	{
		if (this != &other)
		{
			if (m_capacity != other.m_capacity)
			{
				m_capacity = other.m_capacity;
				m_pairs.clear();
				if (m_keys.size() > 0)
				{
					m_keys.erase(0, m_keys.size() - 1);
					m_keys.clear();
				}

				m_keys.reserve(m_capacity);
			}

			m_size = other.m_size;
			for (int i = 0; i < m_size; i++)
			{
				m_pairs.push_tail(other.m_pairs.at(i));
				m_keys.push_back(other.m_pairs.at(i).key);
			}
		}
		return *this;
	}

	ComplexPair<T, N>& operator [] (T key)
	{
		bool bFind = false;
		int i = 0;
		for (i = 0; i < m_size; i++)
		{
			if (m_keys.at(i) == key)
			{
				bFind = true;
				break;
			}
		}

		if (!bFind)
		{
			insert({ key , N() });
			return m_pairs.at(m_size - 1);
		}
		return m_pairs.at(i);
	}

private:

	void Duplicate(T key)
	{
		bool bOverlapKey = false;
		for (int i = 0; i < m_size; i++)
		{
			if (key == m_pairs.at(i).key)
			{
				bOverlapKey = true;
			}
		}

		if (bOverlapKey) throw "Duplicate Key";
	}

};


#endif /* Koh_include_ComplexMap_H */
