#pragma once

#include "ComplexXMLParsor.h"
#include "ComplexVector.h"

namespace ComplexLibrary
{
	class ComplexXMLHandler;
	class ComplexXMLTreeElem;

	typedef ComplexMap<ComplexString, ComplexString> AttributeContainer;
	typedef ComplexVector<ComplexXMLTreeElem*> ElementContainer;

	//	Ʈ�������� xml������ ����
	class ComplexXMLTreeElem
	{
		friend class ComplexXMLHandler; // �ӽ�

	public:

		ComplexXMLTreeElem()
			: m_ElemLevel(0)
			, m_ElemIdx(0)
		{

		}

		virtual ~ComplexXMLTreeElem()
		{
			for (int i = 0; i < (int)m_childElemVector.size(); i++)
			{
				ComplexXMLTreeElem* childElem = m_childElemVector.at(i);
				delete childElem;
				childElem = nullptr;
			}
			m_childElemVector.clear();
		}

		ComplexXMLTreeElem*			m_parentElem;		// �ش� ������Ʈ�� �θ� ������Ʈ��
		AttributeContainer			m_attrMap;			// xml ������Ʈ�� �Ӽ������� ��� ��
		ElementContainer			m_childElemVector;	// xml ������Ʈ�� ���� ������Ʈ���� ��� ����
		ComplexString				m_tagName;			// �ش� ������Ʈ�� �±��̸�
		int							m_ElemLevel;		// ������Ʈ�� ���� 
		int							m_ElemIdx;			// �θ� ������Ʈ�� ���� �ڽ��� ���° ���� ������Ʈ���� ��Ÿ���� ������Ʈ �ε���

	};


	class ComplexXMLHandler
	{

	public:
		ComplexXMLHandler()
			: m_rootElement(new ComplexXMLTreeElem)
		{

		}

		virtual ~ComplexXMLHandler()
		{
			Release();
		}

		/*
		* xml�� �ε��Ͽ� m_rootElement�� ��� ���� ����
		*/
		bool Load(ComplexString strFilePath)
		{
			ComplexXMLParsor mark;

			if (mark.Load(strFilePath) == false)
			{
				Release();
				return false;
			}

			if (m_rootElement == nullptr)
				m_rootElement = new ComplexXMLTreeElem;

			m_strFilePath = strFilePath;
			InitTreeData(mark);

			return true;
		}

		/*
		* m_curElement�� ����Ű���ִ� ��ġ ������Ʈ�� ����
		*/
		ComplexXMLTreeElem* GetCurElemInstance()
		{
			if (m_curElement == nullptr)
				m_curElement = m_rootElement;

			return m_curElement;
		}

		/*
		* �±��̸��� ������Ʈ �ε����� ������Ʈ�� ã��
		* �߰��ϸ� m_curTagElement�� ����
		*/
		bool FindElemPos(ComplexString strTagName, int tagIdx)
		{
			if (m_rootElement)
			{
				if (m_rootElement->m_tagName == strTagName && m_rootElement->m_ElemIdx == tagIdx)
				{
					m_curElement = m_rootElement;
					return true;
				}
				else
				{
					return FindElem(m_rootElement->m_childElemVector, strTagName, tagIdx);
				}
			}
			return false;
		}

		/*
		* Ư�� ������Ʈ�� m_rootElement���� ��� ����
		*/
		bool DeleteCertainElement(ComplexString strTagName)
		{
			if (m_rootElement == nullptr)
				return false;

			return DeleteElement(&m_rootElement->m_childElemVector, strTagName);
		}

		/*
		* �Է��� m_rootElement�� xml ��ο� ������.
		* ��ΰ� ���� �ȵǾ������� load���� �� ������ ��η� ������
		*/
		bool Save(ComplexString strFilePath = "")
		{
			if (m_rootElement == nullptr)
				return false;

			bool bAnotherFile = false;

			if (!strFilePath.IsEmpty())
				bAnotherFile = true;

			ComplexXMLParsor markup;

			markup.AddElement(m_rootElement->m_tagName);
			AttributeContainer attrMap = m_rootElement->m_attrMap;
			AttributeContainer::iterator iter = attrMap.begin();
			while (iter != attrMap.end())
			{
				markup.AddAttribute(iter->value.key, iter->value.value);
				iter++;
			}
			markup.IntoElement();
			ExecuteTreeTag(markup, m_rootElement->m_childElemVector);
			markup.OutOfElement();

			markup.Save(bAnotherFile ? strFilePath : m_strFilePath);

			return true;
		}

		/*
		* m_rootElement���� ������
		* xml�� ������ �� ���
		*/
		void SetRootElement(ComplexXMLTreeElem* overWriteRootElem)
		{
			m_rootElement = overWriteRootElem;
		}

		/*
		* xml�� �ε����� �� ������ ���������� ���
		*/
		ComplexXMLTreeElem* GetRootElement() const
		{
			return m_rootElement;
		}

		/*
		* ��� ������ ����� �Ϸ�Ǿ��� �� �ش� Ʈ��������Ʈ �޸𸮸� ������
		*/
		void Release()
		{
			if (m_rootElement)
			{
				delete m_rootElement;
				m_rootElement = nullptr;
			}

			m_curElement = nullptr;
			m_strFilePath = "";
		}

	private:

		ComplexXMLTreeElem*		 m_rootElement;	// ������ �Է��̳� �ε��� �� ����ϴ� ������ ���������� ��ü

		ComplexXMLTreeElem*		 m_curElement;		// �ε��Ͽ��� �� ã����� �����͸� �߰����� �� ����ϴ� ��ü

		ComplexString			 m_strFilePath;		// �ε��� �� ����Ǵ� xml ���ϰ��

		void InitTreeData(ComplexXMLParsor& mark)
		{
			if (mark.FindElement())
			{
				m_rootElement->m_parentElem = nullptr;
				m_rootElement->m_tagName = mark.GetElementName();
				m_rootElement->m_ElemIdx = 0;
				m_rootElement->m_ElemLevel = 0;
				m_curElement = m_rootElement;
				mark.IntoElement();

				LoadElem(mark, m_rootElement, 1);
				mark.OutOfElement();
			}

		}

		void LoadElem(ComplexXMLParsor& mark, ComplexXMLTreeElem* parentTag, int tagLevel)
		{
			int i = 0;
			while (mark.FindElement())
			{
				ComplexXMLTreeElem* curTag = new ComplexXMLTreeElem;
				AttributeContainer attrMap;

				int j = 0;
				while (mark.GetAttributeName(j).IsEmpty() == false)
				{
					attrMap.insert(mark.GetAttributeName(j), mark.GetAttribute(mark.GetAttributeName(j)));
					j++;
				}
				curTag->m_parentElem = parentTag;
				curTag->m_tagName = mark.GetElementName();
				curTag->m_ElemIdx = i;
				curTag->m_ElemLevel = tagLevel;
				curTag->m_attrMap = attrMap;

				m_curElement = curTag;

				if (mark.HasChildElement())
				{
					mark.IntoElement();
					LoadElem(mark, curTag, tagLevel + 1);
					mark.OutOfElement();
				}
				parentTag->m_childElemVector.push_back(curTag);

				i++;
			}

		}

		bool FindElem(ElementContainer findElemContainer, ComplexString strTagName, int tagIdx)
		{
			for (int i = 0; i < (int)findElemContainer.size(); i++)
			{
				ComplexXMLTreeElem* childElem = findElemContainer.at(i);
				if (childElem->m_tagName == strTagName && childElem->m_ElemIdx == tagIdx)
				{
					m_curElement = childElem;
					return true;
				}
				else
				{
					if (FindElem(childElem->m_childElemVector, strTagName, tagIdx))
					{
						return true;
					}
				}
			}
			return false;
		}

		void ExecuteTreeTag(ComplexXMLParsor& mark, ElementContainer executeTagContainer)
		{
			for (int i = 0; i < (int)executeTagContainer.size(); i++)
			{
				ComplexXMLTreeElem* executeTag = executeTagContainer.at(i);
				AttributeContainer attrMap = executeTag->m_attrMap;

				mark.AddElement(executeTag->m_tagName);
				if (!attrMap.empty())
				{
					AttributeContainer::iterator iter = attrMap.begin();
					while (iter != attrMap.end())
					{
						mark.AddAttribute(iter->value.key, iter->value.value);
						iter++;
					}
				}

				mark.IntoElement();
				ExecuteTreeTag(mark, executeTag->m_childElemVector);
				mark.OutOfElement();
			}
		}

		bool DeleteElement(ElementContainer* tagetContainer, ComplexString strTagName)
		{
			bool bReturn = false;
			int i = 0;

			for (int i = 0; i < (int)tagetContainer->size();)
			{
				ComplexXMLTreeElem* childElem = tagetContainer->at(i);
				bReturn = DeleteElement(&childElem->m_childElemVector, strTagName);
				if (childElem->m_tagName == strTagName)
				{
					tagetContainer->erase(i);
					delete childElem;
					childElem = nullptr;
					bReturn = true;
				}
				else
				{
					i++;
				}
			}
			return bReturn;
		}

	};
}