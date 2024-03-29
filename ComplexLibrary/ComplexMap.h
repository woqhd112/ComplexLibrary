#pragma once

#include <initializer_list>
#include "ComplexSet.h"
#include "ComplexVector.h"
#include "ComplexLinkedList.h"

namespace ComplexLibrary
{
	template <typename T, typename N>
	struct ComplexPair
	{
		T key;
		N value;

		ComplexPair() {}
		ComplexPair(T _key, N _value) { key = _key; value = _value; }
		N operator = (N value)
		{
			this->value = value;
			return this->value;
		}

		ComplexPair& operator = (std::pair<T, N>& pair)
		{
			this->key = pair.first;
			this->value = pair.second;
			return *this;
		}
	};


	template <typename T, typename N>
	class ComplexMap
	{
	private:


		ComplexVector<T> m_keys;
		ComplexLinkedList<ComplexPair<T, N>> m_pairs;

		ComplexLinkedList<ComplexPair<T, N>> m_rpairs;

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

		ComplexMap(ComplexMap<T, N>& ptr)
		{
			clear();
			m_size = ptr.m_size;
			m_capacity = ptr.m_capacity;
			m_keys.reserve(m_capacity);

			for (int i = 0; i < m_size; i++)
			{
				ComplexPair<T, N> pair = ptr.m_pairs.at(i);
				T key = pair.key;
				m_pairs.push_tail(pair);
				m_rpairs.push_head(pair);
				m_keys.push_back(key);
			}
		}

		ComplexMap(ComplexMap<T, N>&& ptr)
		{
			clear();
			m_size = ptr.m_size;
			m_capacity = ptr.m_capacity;
			m_keys.reserve(m_capacity);

			for (int i = 0; i < m_size; i++)
			{
				ComplexPair<T, N> pair = ptr.m_pairs.at(i);
				T key = pair.key;
				m_pairs.push_tail(pair);
				m_rpairs.push_head(pair);
				m_keys.push_back(key);
			}
		}

		ComplexMap(std::initializer_list<std::pair<T, N>> list)
		{
			clear();
			int size = list.size();
			m_capacity = size;
			m_keys.reserve(m_capacity);

			auto iter = list.begin();
			while (iter != list.end())
			{
				std::pair<T, N> iter_pair = *iter;
				m_pairs.push_tail(iter_pair.first, iter_pair.second);
				m_rpairs.push_head(iter_pair.first, iter_pair.second);
				m_keys.push_back(iter_pair.first);
				iter++;
			}
		}

		~ComplexMap()
		{
			m_keys.clear();
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
			m_rpairs.push_head(pair);
			m_size++;
		}

		void insert(T key, N value)
		{
			Duplicate(key);

			if (m_size + 1 > m_capacity)
			{
				m_keys.reserve(m_capacity * 2);
				m_capacity *= 2;

			}
			m_keys.push_back(key);
			m_pairs.push_tail(ComplexPair<T, N>(key, value));
			m_rpairs.push_head(ComplexPair<T, N>(key, value));
			m_size++;
		}

		bool erase(T key)
		{
			if (m_size <= 0)
				throw ComplexIndexOutOfBoundsException("map size is zero.", "ComplexMap", "erase");

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
				return false;

			m_pairs.erase(i);
			m_rpairs.erase(i);
			m_keys.erase(i);
			m_size--;

			return true;
		}

		iterator find(T key)
		{
			if (m_size < 0)
				throw ComplexIndexOutOfBoundsException("map size is error.", "ComplexMap", "find");
			else if (m_size == 0)
				return iterator(nullptr);

			int i = 0;
			bool bFind = false;
			for (i = 0; i < m_size; i++)
			{
				if (m_keys.at(i) == key)
				{
					bFind = true;
					break;
				}
			}
			if (!bFind)
				return end();

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

		iterator rbegin()
		{
			return iterator(m_rpairs.get_head());
		}

		iterator rend()
		{
			return iterator(nullptr);
		}

		// keyset 사용시 key에 담는 클래스에 < operator 구현할 것
		ComplexSet<T> key_set()
		{
			ComplexSet<T> keyset;
			auto key_iter = m_keys.begin();
			while (key_iter != m_keys.end())
			{
				keyset.insert(key_iter->value);
				key_iter++;
			}
			return keyset;
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
			m_rpairs.clear();
		}

		ComplexMap<T, N>& operator = (ComplexMap<T, N>& other)
		{
			clear();
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			m_keys.reserve(m_capacity);

			for (int i = 0; i < m_size; i++)
			{
				ComplexPair<T, N> pair = other.m_pairs.at(i);
				T key = pair.key;
				m_pairs.push_tail(pair);
				m_rpairs.push_head(pair);
				m_keys.push_back(key);
			}

			return *this;
		}

		ComplexMap<T, N>& operator = (std::initializer_list<std::pair<T, N>> list)
		{
			clear();
			int size = list.size();
			m_capacity = size;
			m_keys.reserve(m_capacity);

			auto iter = list.begin();
			while (iter != list.end())
			{
				std::pair<T, N> iter_pair = *iter;
				m_pairs.push_tail(iter_pair.first, iter_pair.second);
				m_rpairs.push_head(iter_pair.first, iter_pair.second);
				m_keys.push_back(iter_pair.first);
				iter++;
			}

			return *this;
		}

		N& operator [] (T key)
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
				return m_pairs.at(m_size - 1).value;
			}
			return m_pairs.at(i).value;
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

			if (bOverlapKey)
				throw ComplexDuplicateException("map insert key is duplicate.", "ComplexMap", "Duplicate");
		}

	};
}
