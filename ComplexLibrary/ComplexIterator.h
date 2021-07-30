#pragma once

#include "ComplexNode.h"

template <typename T>
class ComplexIterator
{
public:

	ComplexIterator(ComplexNode<T>* p = nullptr)
		: m_cur(p)
	{

	}

	~ComplexIterator()
	{

	}

	ComplexIterator& operator ++ ()
	{
		m_cur = m_cur->m_next;
		return *this;
	}

	ComplexIterator operator ++ (int)
	{
		ComplexIterator tmp = *this;
		++*this;
		return tmp;
	}

	ComplexNode<T>& operator * ()
	{
		return *m_cur;
	}

	ComplexNode<T>* operator -> ()
	{
		return m_cur;
	}

	/*T& operator * ()
	{
		return m_cur->value;
	}

	T* operator -> ()
	{
		return &m_cur->value;
	}*/

	bool operator == (const ComplexIterator& eq)
	{
		return m_cur == eq.m_cur;
	}

	bool operator != (const ComplexIterator& eq)
	{
		return m_cur != eq.m_cur;
	}

private:

	ComplexNode<T>* m_cur;

};

