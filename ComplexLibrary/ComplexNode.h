#pragma once

template <typename T>
class ComplexNode
{
	template <typename T> friend class ComplexLinkedList;
	template <typename T> friend class ComplexIterator;
	template <typename T> friend class ComplexQueue;
	template <typename T> friend class ComplexStack;

public:

	T value;
	ComplexNode() {}
	~ComplexNode() {}
	ComplexNode(const T& value, ComplexNode<T>* ptr)
	{
		this->value = value;
		this->m_next = ptr;
	}

	bool has_next()
	{
		return (m_next != nullptr);
	}

	ComplexNode<T>* next()
	{
		return this->m_next;
	}

private:

	ComplexNode<T>* m_next = nullptr;

	/*ComplexNode<T>& operator = (const ComplexNode<T>* ptr)
	{
		this->value = ptr->value;
		this->next = ptr->next;
		return *this;
	}*/
};
