#pragma once

#include "ComplexNode.h"

template <typename T>
class ComplexStack
{
public:

	ComplexStack() : m_head(nullptr), m_tail(nullptr), m_size(0)
	{
	
	}

	~ComplexStack()
	{
	
	}

	void push(T value)
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
			node->m_next = m_head;
			m_head = node;
		}
	}

	T& top() const
	{
		return m_head->value;
	}

	void pop()
	{
		ComplexNode<T>* old = m_head;

		if (m_head == nullptr)
			return;

		m_head = m_head->m_next;
		delete old;
		old = nullptr;

		m_size--;

		if (m_size <= 0)
		{
			m_head = nullptr;
			m_tail = nullptr;
			m_size = 0;
		}
	}

	int size()
	{
		return m_size;
	}

	bool empty()
	{
		return (m_size <= 0);
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

private:

	ComplexNode<T>* m_head;
	ComplexNode<T>* m_tail;
	int m_size;

};