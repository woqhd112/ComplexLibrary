#pragma once

#include "ComplexStringTokenizer.h"

namespace ComplexLibrary
{
	class ComplexScanner
	{
	public:

		ComplexScanner()
		{

		}

		virtual ~ComplexScanner()
		{
			m_st.Clear();
		}

		bool HasNext()
		{
			return m_st.HasNextToken();
		}

		// buf�� �ް� ' '�� �߶� �����ϰ��ִٰ� ��ū���� �����ϸ� ���� ��ū�� �����ϰ� �������������� ���Ӱ� �޴´�.
		ComplexString Next()
		{
			ComplexString in = "";
			ComplexString returnBuf = in;

			if (m_st.HasNextToken() == false)
			{
				char buf[2048];
				scanf_s("%s", buf, 2048);
				in = buf;
				m_st.ApplyStringTokenize(in, ' ');
			}

			returnBuf = m_st.NextToken();

			return returnBuf;
		}

		// buf�� �ް� �״�� �����Ѵ�.
		// nextline �ϸ� next���� �� ��ū�� ���� �ʱ�ȭ�ϴ°� �´°ǰ�? �ϴ� �ʱ�ȭ�Ѵ�.
		ComplexString NextLine()
		{
			m_st.Clear();

			char buf[2048];

			scanf_s("%s", buf, 2048);

			ComplexString in = buf;

			return in;
		}

	private:

		ComplexStringTokenizer m_st;

	};

}
