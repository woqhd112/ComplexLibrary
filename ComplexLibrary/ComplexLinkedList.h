#pragma once

#include "ComplexIterator.h"


template <typename T>
class ComplexLinkedList
{
private:
	ComplexNode<T>* m_head;
	ComplexNode<T>* m_tail;
	int m_size;

public:

	typedef ComplexIterator<T> iterator;

	ComplexLinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}
	~ComplexLinkedList() { clear(); }

	void push_tail(T value)
	{
		ComplexNode<T>* node = new ComplexNode<T>;
		m_size++;

		node->value = value;
		node->m_next = nullptr;

		if (m_head == nullptr)
		{
			m_head = node;
			m_tail = node;
		}
		else
		{
			m_tail->m_next = node;
			m_tail = m_tail->m_next;
		}
	}

	void push_head(T value)
	{
		ComplexNode<T>* node = new ComplexNode<T>;
		m_size++;

		if (m_head != nullptr)
		{
			node->m_next = m_head;
		}
		node->value = value;
		m_head = node;
	}

	bool push(int index, T value)
	{
		if (m_size < index) return false;	// skip tail..
		if (index < 0) return false;	// not treatment index

		if (m_head == nullptr || m_size == index)
		{
			push_tail(value);
			return true;
		}

		ComplexNode<T>* moveNode = m_head;
		ComplexNode<T>* containNode = moveNode;
		ComplexNode<T>* node = new ComplexNode<T>;

		node->value = value;
		node->m_next = nullptr;

		int cnt = 0;
		while (moveNode != nullptr)
		{
			if (cnt == index)
			{
				containNode = moveNode;
				moveNode = node;
				moveNode->m_next = containNode;
				break;
			}
			moveNode = moveNode->m_next;
			cnt++;
		}

		m_size++;

		return true;
	}

	void clear()
	{
		ComplexNode<T>* deleteNode = m_head;

		while (deleteNode != nullptr)
		{
			m_head = deleteNode->m_next;
			delete deleteNode;
			deleteNode = m_head;
		}
		m_head = nullptr;
		m_tail = nullptr;
		m_size = 0;
	}

	bool erase(int index)
	{
		if (m_size <= index) return false;	// index out of bound 
		if (index < 0) return false;	// not treatment index

		ComplexNode<T>* findNode = m_head;
		ComplexNode<T>* nextNode = findNode;

		int cnt = 0;
		while (findNode != nullptr)
		{
			if (cnt == index)
			{
				break;
			}
			else
			{
				nextNode = findNode;
				findNode = findNode->m_next;
			}

			cnt++;
		}

		if (findNode == nullptr)
		{
			return false;
		}
		else
		{
			m_size--;
			nextNode->m_next = findNode->m_next;
			delete findNode;
		}


		return true;
	}

	ComplexNode<T>* get_at(int index)
	{
		ComplexNode<T>* findNode = m_head;

		int cnt = 0;
		while (findNode != nullptr)
		{
			if (cnt == index)
			{
				break;
			}
			findNode = findNode->m_next;
			cnt++;
		}

		return findNode;
	}

	ComplexNode<T>* get_head()
	{
		return m_head;
	}

	ComplexNode<T>* get_tail()
	{
		return m_tail;
	}

	T& at(int index)
	{
		ComplexNode<T>* findNode = m_head;

		int cnt = 0;
		while (findNode != nullptr)
		{
			if (cnt == index)
			{
				break;
			}
			findNode = findNode->m_next;
			cnt++;
		}

		return findNode->value;
	}

	iterator begin()
	{
		return iterator(m_head);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	T& head()
	{
		return m_head->value;
	}

	T& tail()
	{
		return m_tail->value;
	}

	bool empty()
	{
		return (m_size <= 0);
	}

	int size()
	{
		return m_size;
	}
};
