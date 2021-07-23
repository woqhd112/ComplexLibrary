#ifndef Koh_include_ComplexIterator_H  
#define Koh_include_ComplexIterator_H 1

template <typename T>
struct ComplexNode
{
	T value;
	struct ComplexNode<T>* next = nullptr;
	ComplexNode() {}
	ComplexNode(const T& value, ComplexNode<T>* ptr)
	{
		this->value = value;
		this->next = ptr;
	}
};

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
		m_cur = m_cur->next;
		return *this;
	}

	ComplexIterator operator ++ (int)
	{
		ComplexIterator tmp = *this;
		++*this;
		return tmp;
	}

	T& operator * ()
	{
		return m_cur->value;
	}

	T* operator -> ()
	{
		return &m_cur->value;
	}

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

#endif /* Koh_include_ComplexIterator_H */
