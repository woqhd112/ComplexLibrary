#pragma once

#include "ComplexFile.h"
#include "ComplexXMLNode.h"

#define OPEN_TAG "<"
#define CLOSE_TAG ">"
#define DEFINE_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
#define ANNOTATION_OPEN_TAG "<!--"
#define ANNOTATION_CLOSE_TAG "-->"


class ComplexXMLParsor : public ComplexFile
{
public:

	ComplexXMLParsor()
	{
		m_processElem = &m_rootElem;
		m_positionElem = m_processElem;
	}

	~ComplexXMLParsor()
	{
		m_processElem = nullptr;
		m_positionElem = nullptr;
	}

	bool Load(ComplexString path)
	{
		if (!Read(path, m_buf))
			return false;

		if (!ReadHeader())
			return false;

		EncodeDoc();

		return true;
	}

	bool HasChildElement()
	{
		return m_processElem->HasChildElement();
	}

	bool FindElement()
	{
		m_processElem = m_positionElem;
		if (m_processElem == nullptr)
			return false;

		m_positionElem = m_positionElem->GetNextElement();

		return true;
	}

	bool FindElement(ComplexString findelemName)
	{
		if (m_processElem == nullptr)
			return false;

		if (m_processElem->GetElementName() == findelemName)
			return true;

		bool bFind = false;
		while (m_processElem->HasNextElement())
		{
			m_processElem = m_processElem->GetNextElement();

			if (m_processElem->GetElementName() == findelemName)
			{
				bFind = true;
				break;
			}
		}

		m_positionElem = m_processElem;

		return bFind;
	}

	ComplexString& GetDoc()
	{
		return m_doc;
	}

	bool Save(ComplexString strPath)
	{
		if (m_header.IsEmpty())
			m_header = DEFINE_HEADER;

		if (m_header != DEFINE_HEADER)
			return false;

		ComplexString resultDoc = "";
		DecodeDoc(resultDoc);
		m_doc = resultDoc;

		if (resultDoc.IsEmpty())
			return false;

		if (Write(strPath, resultDoc) == false)
			return false;

		return true;
	}

	void ResetCurrnetPosition()
	{
		if (m_processElem == nullptr)
			return;

		if (m_processElem->HasParentElement() == false)
			m_processElem = &m_rootElem;

		m_processElem = m_processElem->GetParentElement();
		m_processElem = m_processElem->GetChildElement();

		m_positionElem = m_processElem;
	}

	void ResetAllPosition()
	{
		m_processElem = &m_rootElem;
		m_positionElem = m_processElem;
	}

	void AddElement(ComplexString elemName, ComplexString elemValue = "")
	{
		m_positionElem = m_processElem;
		if (m_processElem == nullptr)
			return;

		ComplexXMLNode* newElem = new ComplexXMLNode;
		newElem->SetElementName(elemName);

		while (m_positionElem->HasNextElement())
		{
			m_positionElem = m_positionElem->GetNextElement();
		}
		m_processElem = m_positionElem;
		m_processElem->SetNextElement(newElem);
	}

	bool SetElementValue(ComplexString elemValue)
	{
		if (m_processElem == nullptr)
			return false;

		m_processElem->SetElementValue(elemValue);
		return true;
	}

	bool AddAttribute(ComplexString attrName, ComplexString attrValue)
	{
		if (m_processElem == nullptr)
			return false;

		m_processElem->AppendAttribute(attrName, attrValue);

		return true;
	}

	bool SetAttribute(ComplexString attrName, ComplexString attrValue)
	{
		if (m_processElem == nullptr)
			return false;

		m_processElem->AppendAttribute(attrName, attrValue);

		return true;
	}

	ComplexString GetAttribute(ComplexString attrName)
	{
		if (m_processElem == nullptr)
			return "";

		return m_processElem->GetAttribute(attrName);
	}

	ComplexString GetAttributeName(int idx)
	{
		if (m_processElem == nullptr)
			return "";

		return m_processElem->FindKey(idx);
	}

	ComplexString GetElementName()
	{
		if (m_processElem == nullptr)
			return "";

		return m_processElem->GetElementName();
	}

	ComplexString GetElementValue()
	{
		if (m_processElem == nullptr)
			return "";

		return m_processElem->GetElementValue();
	}

	bool IntoElement()
	{
		if (m_processElem == nullptr)
			return false;

		if (m_processElem->HasChildElement() == false)
			return false;

		m_processElem = m_processElem->GetChildElement();
		m_positionElem = m_processElem;
		return true;
	}

	bool OutOfElement()
	{
		if (m_processElem == nullptr)
			return false;

		if (m_processElem->HasParentElement() == false)
			return false;

		m_processElem = m_processElem->GetParentElement();
		m_positionElem = m_processElem;
		return true;
	}

	bool RemoveElement()
	{
		if (m_processElem == nullptr)
			return false;

		m_positionElem = m_processElem;

		if (m_positionElem->HasNextElement())
			m_positionElem = m_positionElem->GetNextElement();
		else
		{
			if (m_positionElem->HasParentElement())
			{
				m_positionElem = m_positionElem->GetParentElement();
				m_positionElem = m_positionElem->GetChildElement();
				while (m_positionElem->HasNextElement())
				{
					m_positionElem = m_positionElem->GetNextElement();
				}
			}
			else
				m_positionElem = &m_rootElem;
		}

		ComplexXMLNode* removeElem = m_processElem;
		delete removeElem;
		removeElem = nullptr;

		m_processElem = m_positionElem;
	}

private:

	void EncodeDoc()
	{
		InputDoc(&m_rootElem, nullptr, m_doc);
		m_processElem = &m_rootElem;
	}

	void DecodeDoc(ComplexString& resultDoc)
	{
		m_processElem = &m_rootElem;
		OutputDoc(m_processElem, resultDoc);
	}

	void OutputDoc(ComplexXMLNode* currentElem, ComplexString& resultDoc)
	{
		ComplexString depthTab = "";

		resultDoc.Format("%s", m_header.GetBuffer());	// append header

		if (currentElem == nullptr)
			return;

		resultDoc += "\n";
		AppendElement(currentElem, resultDoc, depthTab);
		while (currentElem->HasNextElement())
		{
			currentElem = currentElem->GetNextElement();
			resultDoc += "\n";
			AppendElement(currentElem, resultDoc, depthTab);
		}
	}

	void AppendElement(ComplexXMLNode* elem, ComplexString& appendDoc, ComplexString& depthTab)
	{
		appendDoc += depthTab;	// append depth tab
		appendDoc += "<";	// open tag
		appendDoc += elem->GetElementName();	// append elem name

		ComplexXMLNode::ComplexAttributes* attr = elem->GetAttributes();
		if (attr->size() > 0)	// append attr
			AppendAttibute(attr, appendDoc);

		if (elem->HasChildElement())	// append child elem
		{
			appendDoc += ">";	// append child close tag
			ComplexXMLNode* childElem = elem->GetChildElement();

			appendDoc += "\n";	// divide new child elem 
			ComplexString tab = depthTab + "\t";	// calc child depth tab
			AppendElement(childElem, appendDoc, tab);
			while (childElem->HasNextElement())
			{
				childElem = childElem->GetNextElement();
				appendDoc += "\n";	// divide new child elem 
				ComplexString tab = depthTab + "\t";	// calc child depth tab
				AppendElement(childElem, appendDoc, tab);
			}
			appendDoc.AppendFormat("\n%s</%s>", depthTab.GetBuffer(), elem->GetElementName().GetBuffer());
		}
		else
		{
			appendDoc += "/>";	// append not child close tag
		}
	}

	void AppendAttibute(ComplexXMLNode::ComplexAttributes* attr, ComplexString& appendDoc)
	{
		ComplexXMLNode::ComplexAttributes::iterator iter = attr->begin();
		while (iter != attr->end())
		{
			ComplexString attrValue;
			attrValue.Format("\"%s\"", iter->value.value.GetBuffer());

			appendDoc += " ";
			appendDoc += iter->value.key;	// append attr name
			appendDoc += "=";
			appendDoc += attrValue;	// append attr value
			iter++;
		}
	}

	ComplexString GetText(int begin_idx, int end_idx, ComplexString& doc)
	{
		if (end_idx - begin_idx <= 0)
			return "";
		int text_size = end_idx - begin_idx + 1;
		char* text_buf = new char[text_size + 1];
		int buf_idx = 0;
		for (int i = begin_idx; i <= end_idx; i++)
		{
			text_buf[buf_idx] = doc[i];
			buf_idx++;
		}
		text_buf[text_size] = '\0';
		ComplexString text = text_buf;
		delete[] text_buf;

		return text;
	}

	bool SplitAttr(ComplexXMLNode* currentElem, ComplexString& buf)
	{
		const char* buf_buffer = buf.GetBuffer();

		if (buf.Find("<") != -1 || buf.Find(">") != -1)
			return false;

		char attr_name[256];
		char attr_value[256];
		bool isValue = false;
		bool one_attr_end = false;

		int name_cnt = 0;
		int value_cnt = 0;
		for (int i = 0; i < buf.GetLength(); i++)
		{
			if (buf_buffer[i] == '=')
			{
				continue;
			}
			else if (buf_buffer[i] == '"')
			{
				if (!isValue)
					isValue = true;
				else
				{
					isValue = false;
					one_attr_end = true;
				}
			}
			else if (isValue)
			{
				attr_value[value_cnt] = buf_buffer[i];
				value_cnt++;
			}
			else
			{
				attr_name[name_cnt] = buf_buffer[i];
				name_cnt++;
			}

			if (one_attr_end)
			{
				attr_name[name_cnt] = '\0';
				attr_value[value_cnt] = '\0';
				ComplexString attrName = attr_name;
				attrName.Trim();
				ComplexString attrValue = attr_value;
				attrValue.Trim();
				currentElem->AppendAttribute(attrName, attrValue);
				one_attr_end = false;
				memset(attr_name, 0, sizeof(char) * 256);
				memset(attr_value, 0, sizeof(char) * 256);
				name_cnt = 0;
				value_cnt = 0;
			}
		}

		return true;
	}

	void VaildateAnnotation(ComplexString& doc)
	{
		// �Լ� ȣ����ġ ���� �� ����
		int open_tag = doc.Find(ANNOTATION_OPEN_TAG);
		int close_tag = doc.Find(ANNOTATION_CLOSE_TAG);

		ComplexString annotation_text = GetText(open_tag + 3, close_tag + 2, m_doc);
	}

	bool ValidateTag(ComplexString& checkDoc)
	{
		int open_tag = checkDoc.Find("<");
		int close_tag = checkDoc.Find(">");

		if (open_tag == -1 && close_tag == -1)
			return false;
		else if (open_tag != -1 && close_tag == -1)
		{
			throw "Not Close Tag";
		}
		else if (open_tag == -1 && close_tag != -1)
		{
			throw "Not Open Tag";
		}

		return true;
	}

	void InputDoc(ComplexXMLNode* currentElem, ComplexXMLNode* parentElem, ComplexString tag)
	{
		if (tag == "")
			return;

		if (ValidateTag(tag) == false)
			return;

		bool bNotChild = false;
		int close_tag = tag.Find(">");

		const char* tag_buf = tag.GetBuffer();
		if (tag_buf[close_tag - 1] == '/')
			bNotChild = true;

		ComplexString elemName = GetElemName(tag);
		if (elemName == "")
			return;

		currentElem->SetParentElement(parentElem);
		currentElem->SetElementName(elemName);

		// /> ó��
		if (bNotChild)
		{
			bNotChild = true;
			// </ �� �ƴ϶� /> �� �±�
			int end_tag_idx = close_tag - 1;
			ComplexString elemTag = GetText(0, end_tag_idx, tag);
			elemTag.Trim();

			// �Ӽ� �Ľ�
			elemTag.RemoveAll("<");
			elemTag.RemoveAll(">");
			elemTag.Remove(elemName);
			elemTag.Trim();
			SplitAttr(currentElem, elemTag);
		}
		// </ ó��
		else
		{
			ComplexString endTag;
			endTag.Format("/%s", elemName.GetBuffer());
			close_tag = tag.Find(endTag);

			if (close_tag != -1)
			{
				// </ ���Ե� �±�  
				int end_tag_idx = close_tag - 2;	// length of <TagName ...> ...
				close_tag = close_tag + elemName.GetLength() + 1;		// length of <TagName ...> ... </TagName> 
				ComplexString elemTag = GetText(0, end_tag_idx, tag);
				elemTag.Trim();

				// �Ӽ� �Ľ�
				int attr_end_idx = elemTag.Find(">");
				ComplexString attr = GetText(0, attr_end_idx, elemTag);
				attr.RemoveAll("<");
				attr.RemoveAll(">");
				attr.Remove(elemName);
				attr.Trim();
				SplitAttr(currentElem, attr);

				// �ڽ� �Ľ� (�ڽ��� ������ ������ �±� �˻�� ���ص���)
				ComplexString childElemBuf = GetText(attr_end_idx + 1, end_tag_idx, tag);

				if (childElemBuf != "")
				{
					childElemBuf.Trim();

					// value �Ľ� elem�� value..
					// <TagName> value.. </TagName>
					// value�� �����ϸ� child�� ������ �� ����..
					int child_open_tag = childElemBuf.Find("<");
					int child_close_tag = childElemBuf.Find(">");

					// exist value and not exist child..
					if (child_open_tag == -1 && child_close_tag == -1)
					{
						currentElem->SetElementValue(childElemBuf);
					}
					// not exist value and exist child..
					else
					{
						ComplexXMLNode* childElem = new ComplexXMLNode;
						InputDoc(childElem, currentElem, childElemBuf);
						currentElem->SetChildElement(childElem);
					}
				}
			}
			else
				throw "Not Close Tag";
		}


		// ó�� �Ϸ��� �±� ���� ģ�� �±� ó��
		ComplexString nextElemBuf = GetText(close_tag + 1, tag.GetLength(), tag);
		// ���� ó�� Ʈ�� ��..
		if (!nextElemBuf.IsEmpty())
		{
			nextElemBuf.Trim();
			ComplexXMLNode* nextElem = new ComplexXMLNode;
			InputDoc(nextElem, parentElem, nextElemBuf);
			nextElem->SetParentElement(parentElem);
			currentElem->SetNextElement(nextElem);
		}
	}

	ComplexString GetElemName(ComplexString doc)
	{
		int open_tag_idx = doc.Find(OPEN_TAG);
		int elem_name_idx = doc.Find(" ");
		int close_tag_idx = doc.Find(CLOSE_TAG);

		if (close_tag_idx <= elem_name_idx)
		{
			if (doc.GetBuffer()[close_tag_idx - 1] == '/')
			{
				close_tag_idx -= 1;
			}
			elem_name_idx = close_tag_idx;
		}

		int elem_size = elem_name_idx - open_tag_idx - 1;
		char* elem_name = new char[elem_size + 1];
		int buf_idx = 0;
		for (int i = open_tag_idx + 1; i <= elem_name_idx - 1; i++)
		{
			elem_name[buf_idx] = doc[i];
			buf_idx++;
		}
		elem_name[elem_size] = '\0';
		ComplexString elemName = elem_name;
		delete[] elem_name;

		return elemName;
	}

	bool ReadHeader()
	{
		const char* read_buf = m_buf.GetBuffer();

		// <?xml> minimum length
		if (m_buf.GetLength() < 6)
			return false;

		// xml header parse
		if (read_buf[0] == 32 || read_buf[0] == 9)
			return false;

		if ((read_buf[0] == 13 && read_buf[1] == 10) ||	// window crlf
			(read_buf[0] == 10) ||						// unix lf
			(read_buf[0] == 13))						// mac cr	
			return false;

		// parse header <?xml
		if (read_buf[0] != 60 || read_buf[1] != 63 || read_buf[2] != 120 || read_buf[3] != 109 || read_buf[4] != 108)
			return false;

		int find_first_close_tag_idx = 0;
		for (find_first_close_tag_idx = 0; find_first_close_tag_idx < m_buf.GetLength(); find_first_close_tag_idx++)
		{
			if (m_buf[find_first_close_tag_idx] == 62)
				break;
		}

		int header_size = find_first_close_tag_idx + 1;
		int doc_sise = m_buf.GetLength() - header_size;
		char* header_buf = new char[header_size + 1];	// last null
		char* doc_buf = new char[doc_sise + 1];	// last null

		for (int i = 0; i < header_size; i++)
		{
			header_buf[i] = m_buf[i];
		}
		header_buf[header_size] = '\0';

		for (int i = 0; i < doc_sise; i++)
		{
			doc_buf[i] = m_buf[i + header_size];
		}
		doc_buf[doc_sise] = '\0';

		m_header = header_buf;
		m_doc = doc_buf;
		m_doc.RemoveAll("\n");	// header next \n delete
		m_doc.Trim();

		delete[] header_buf;
		delete[] doc_buf;

		return true;
	}

private:

	ComplexXMLNode m_rootElem;
	ComplexXMLNode* m_processElem;
	ComplexXMLNode* m_positionElem;


	ComplexString m_buf;
	ComplexString m_header;
	ComplexString m_doc;

};