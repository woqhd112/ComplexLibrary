#pragma once

#include "ComplexFile.h"
#include "ComplexMap.h"
#include "ComplexVector.h"

#define OPEN_TAG		"<"
#define CLOSE_TAG		">"

class ComplexXMLParsor : public ComplexFile
{
private:

	struct ComplexElement
	{
		ComplexString element_name = "";
		ComplexMap<ComplexString, ComplexString> attribute_map;
		ComplexVector<ComplexElement*> child_element;
	};

public:

	ComplexXMLParsor()
	{
		m_rootElement = new ComplexElement;
	}

	~ComplexXMLParsor()
	{

	}

	bool Load(ComplexString path)
	{
		if (!Read(path, m_buf))
			return false;

		if (!ReadHeader())
			return false;

		ParseDoc();

		return true;
	}

private:

	void ParseDoc()
	{
		ComplexString elem = FindElem(m_doc);

		AddElem(m_rootElement, elem);

	}

	bool AddElem(ComplexElement* appendElem, ComplexString elem)
	{
		int token_idx = elem.Find(" ");

		if (token_idx <= 0)
			return false;

		ComplexVector<ComplexString> split_data;

		bool success_split = elem.Split(split_data, ' ');

		// not attr
		if (!success_split)
		{
			appendElem->element_name = elem;
		}
		else
		{
			bool attr_error = false;
			appendElem->element_name = split_data.at(0);	// space split first index is element name

			attr_error = AddAttr(split_data, appendElem->attribute_map);

			if (!attr_error)
			{
				return false;
			}
		}

		return true;
	}

	bool AddAttr(ComplexVector<ComplexString> datas, ComplexMap<ComplexString, ComplexString>& attr_map)
	{
		bool attr_error = false;
		for (int i = 1; i < datas.size(); i++)
		{
			ComplexString pair_attribute = datas.at(i);
			const char* pair_buf = pair_attribute.GetBuffer();
			int attr_token_idx = pair_attribute.Find("=");

			if (attr_token_idx <= 0)
			{
				attr_error = true;
				break;
			}

			int attr_name_size = attr_token_idx;
			int attr_value_size = pair_attribute.GetLength() - attr_token_idx - 1;

			char* attr_name = new char[attr_name_size + 1];
			char* attr_value = new char[attr_value_size + 1];

			for (int i = 0; i < attr_name_size; i++)
			{
				attr_name[i] = pair_buf[i];
			}
			attr_name[attr_name_size] = '\0';

			for (int i = 0; i < attr_value_size; i++)
			{
				attr_value[i] = pair_buf[i + attr_name_size + 1];
			}
			attr_value[attr_value_size] = '\0';

			ComplexString attr_name_buf = attr_name;
			ComplexString attr_value_buf = attr_value;
			// "" delete, save only value
			attr_value_buf.RemoveAll("\"");

			delete[] attr_name;
			delete[] attr_value;

			attr_map.insert(attr_name_buf, attr_value_buf);
		}

		// confuse attr
		if (attr_error)
			return false;

		return true;
	}

	ComplexString FindElem(ComplexString buf)
	{
		int open_tag_idx = m_doc.Find(OPEN_TAG);
		int eq_idx = m_doc.Find(CLOSE_TAG);

		// whether close tag is '>' or '\>'
		if (m_doc[eq_idx - 1] == '\\')
		{
			eq_idx -= 1;
		}

		int elem_size = eq_idx - open_tag_idx - 1;
		char* elem_buf = new char[elem_size + 1];
		int buf_idx = 0;
		for (int i = open_tag_idx + 1; i < eq_idx - 1; i++)
		{
			elem_buf[buf_idx] = m_buf[i];
			buf_idx++;
		}
		elem_buf[elem_size] = '\0';

		ComplexString elem = elem_buf;

		delete[] elem_buf;

		return elem;
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
		m_doc.Remove("\n");	// header next \n delete

		delete[] header_buf;
		delete[] doc_buf;

		return true;
	}

private:


	ComplexString m_buf;
	ComplexString m_header;
	ComplexString m_doc;

	ComplexElement* m_rootElement;
};
