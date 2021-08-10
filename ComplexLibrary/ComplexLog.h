#pragma once

#include "ComplexFile.h"
#include "ComplexUtility.h"

class ComplexFormat
{
	friend class ComplexPrintLog;
	friend class ComplexFileLog;
public:

	ComplexFormat()
	{

	}

	~ComplexFormat()
	{

	}

private:

	ComplexString GetFormatString(const char* format, va_list args)
	{
		ComplexString returnBuf;

		int nformatData;
		char* strFormatData;
		double dFormatData;
		bool open_format = false;
		bool use_empty_zero = false;
		bool end_dot_total = false;
		int totalsize = 0;
		int decimalsize = 0;
		char total_buf[4096];
		char decimal_buf[4096];
		while (*format)
		{
			if (*format == '%')
			{
				open_format = true;
				if (*(format + 1) == '0')
				{
					use_empty_zero = true;
					format++;
				}
				else if (*(format + 1) == '%')
				{
					format++;
					returnBuf.AppendFormat("%c", *format);
					putc(*format, stdout);
					open_format = false;
				}
			}
			else if (*format == 'c')
			{
				if (open_format)
				{
					nformatData = __crt_va_arg(args, int);
					char* buf = new char[4096];

					total_buf[totalsize] = '\0';
					int total_len = ComplexUtility::StringToInt(total_buf);
					GetStringToChar(buf, nformatData, use_empty_zero, total_len);
					returnBuf.AppendFormat("%s", buf);

					delete[] buf;
					memset(total_buf, 0, sizeof(char) * 4096);
					memset(decimal_buf, 0, sizeof(char) * 4096);
					open_format = false;
					use_empty_zero = false;
					end_dot_total = false;
					totalsize = 0;
					decimalsize = 0;
				}
				else
					returnBuf.AppendFormat("%c", *format);
			}
			else if (*format == 'd')
			{
				if (open_format)
				{
					nformatData = __crt_va_arg(args, int);
					char* buf = new char[4096];

					total_buf[totalsize] = '\0';
					int total_len = ComplexUtility::StringToInt(total_buf);
					GetStringToInt(buf, nformatData, use_empty_zero, total_len);
					returnBuf.AppendFormat("%s", buf);

					delete[] buf;
					memset(total_buf, 0, sizeof(char) * 4096);
					memset(decimal_buf, 0, sizeof(char) * 4096);
					open_format = false;
					use_empty_zero = false;
					end_dot_total = false;
					totalsize = 0;
					decimalsize = 0;
				}
				else
					returnBuf.AppendFormat("%c", *format);
			}
			else if (*format == 's')
			{
				if (open_format)
				{
					strFormatData = __crt_va_arg(args, char*);
					char* buf = new char[4096];

					total_buf[totalsize] = '\0';
					int total_len = ComplexUtility::StringToInt(total_buf);
					GetStringToString(buf, strFormatData, use_empty_zero, total_len);
					returnBuf.AppendFormat("%s", buf);

					delete[] buf;
					memset(total_buf, 0, sizeof(char) * 4096);
					memset(decimal_buf, 0, sizeof(char) * 4096);
					open_format = false;
					use_empty_zero = false;
					end_dot_total = false;
					totalsize = 0;
					decimalsize = 0;
				}
				else
					returnBuf.AppendFormat("%c", *format);
			}
			else if (*format == 'f')
			{
				if (open_format)
				{
					dFormatData = __crt_va_arg(args, double);
					char* buf = new char[4096];

					total_buf[totalsize] = '\0';
					decimal_buf[decimalsize] = '\0';
					int total_len = ComplexUtility::StringToInt(total_buf);
					int decimal_len = ComplexUtility::StringToInt(decimal_buf);
					GetStringToDouble(buf, dFormatData, use_empty_zero, total_len, decimal_len);
					returnBuf.AppendFormat("%s", buf);

					delete[] buf;
					memset(total_buf, 0, sizeof(char) * 4096);
					memset(decimal_buf, 0, sizeof(char) * 4096);
					open_format = false;
					use_empty_zero = false;
					end_dot_total = false;
					totalsize = 0;
					decimalsize = 0;
				}
				else
					returnBuf.AppendFormat("%c", *format);
			}
			else if (*format == '.')
			{
				if (open_format)
				{
					end_dot_total = true;
				}
				else
					returnBuf.AppendFormat("%c", *format);
			}
			else if (*format == '1' || *format == '2' || *format == '3' ||
				*format == '4' || *format == '5' || *format == '6' ||
				*format == '7' || *format == '8' || *format == '9' ||
				*format == '0')
			{
				if (open_format)
				{
					if (end_dot_total)
					{
						decimal_buf[decimalsize] = *format;
						decimalsize++;
					}
					else
					{
						total_buf[totalsize] = *format;
						totalsize++;
					}
				}
				else
					returnBuf.AppendFormat("%c", *format);
			}
			else
				returnBuf.AppendFormat("%c", *format);

			format++;
		}

		return returnBuf;
	}

	void GetStringToDouble(char* buf, double value, bool use_empty_zero, int total_size, int decimal_size)
	{
		int before_size = 0;
		int after_size = 0;
		int dot_defore = static_cast<int>(value);
		int calc_before = dot_defore;

		while (calc_before)
		{
			calc_before /= 10;
			before_size++;
		}

		double dot_after = value - static_cast<double>(dot_defore);
		unsigned long long floor_remainder_dot_after = static_cast<unsigned long long>(dot_after);

		while (dot_after != floor_remainder_dot_after)
		{
			dot_after *= 10;
			after_size++;
			floor_remainder_dot_after = static_cast<unsigned long long>(dot_after);
		}

		char* integer_part = new char[4096];
		char* double_part = new char[4096];
		char dot = '.';
		int before_total = 0;
		// ��ü ������ -1�� . ���ڵ� ���̿� ���ԵǱ� ����
		// (��ü ������ - 1) - �Ǽ� ������(���� ������) �� ���� ������� ���� ��� -> �����κ��� ���� ����ϰ� �Ǽ� �κ��� decimal_size ��ŭ ���
		if (abs((total_size - 1) - decimal_size) < before_size)
		{
			before_total = before_size;
			GetStringToInt(integer_part, dot_defore, use_empty_zero, before_total);
		}
		else
		{
			if (total_size == 0)
				before_total = before_size;
			else
				before_total = (total_size - 1) - decimal_size;
			GetStringToInt(integer_part, dot_defore, use_empty_zero, before_total);
		}
		// ������Ʈ decimal_size �� after_size�� ó��
		// ���� �Ǽ��κ��� �� Ŭ��� ������ŭ ¥��.
		if (decimal_size < after_size)
		{
			// decimal_size �����ŭ ©�� ���
			GetStringToInt(double_part, floor_remainder_dot_after, use_empty_zero, after_size);
			if (decimal_size == 0)
				decimal_size = after_size;

			double_part[decimal_size] = '\0';
		}
		// ���� �Ǽ��κ��� �� ������� �ڿ� Ư�� ���� �߰�
		else
		{
			// decimal_size - after_size ������ŭ Ư�� ���� �߰�
			GetStringToInt(double_part, floor_remainder_dot_after, use_empty_zero, after_size);
			if (decimal_size == 0)
				decimal_size = after_size;

			for (int i = after_size; i < decimal_size; i++)
			{
				if (use_empty_zero)
					double_part[i] = '0';
				else
					double_part[i] = ' ';
			}
			double_part[after_size] = '\0';
		}

		for (int i = 0; i < before_total; i++)
		{
			buf[i] = integer_part[i];
		}
		buf[before_total] = dot;
		for (int i = 0; i < decimal_size; i++)
		{
			buf[i + before_total + 1] = double_part[i];
		}
		buf[before_total + 1 + decimal_size] = '\0';

		delete[] integer_part;
		delete[] double_part;
	}

	void GetStringToString(char* buf, char* value, bool use_empty_zero, int total_size)
	{
		if (value == NULL)
			return;

		int len = static_cast<int>(strlen(value));

		int empty_count = 0;
		if (total_size >= len)
		{
			empty_count = total_size - len;
		}

		for (int i = 0; i < empty_count; i++)
		{
			if (use_empty_zero)
				buf[i] = '0';
			else
				buf[i] = ' ';
		}
		int j = 0;
		for (int i = empty_count; i < total_size; i++)
		{
			buf[i] = value[j];
			j++;
		}
		buf[total_size] = '\0';
	}

	void GetStringToChar(char* buf, char value, bool use_empty_zero, int total_size)
	{
		for (int i = 0; i < total_size - 1; i++)
		{
			if (use_empty_zero)
				buf[i] = '0';
			else
				buf[i] = ' ';
		}
		if (total_size > 0)
		{
			buf[total_size - 1] = value;
			buf[total_size] = '\0';
		}
		else
		{
			buf[0] = value;
			buf[1] = '\0';
		}
	}

	void GetStringToInt(char* buf, unsigned long long value, bool use_empty_zero, int total_size)
	{
		unsigned long long tmp = value;
		int count = 0;
		char* newBuf = new char[4096];
		while (tmp)
		{
			int remainder = tmp % 10;
			newBuf[count] = remainder + 48;

			tmp /= 10;
			count++;
		}

		if (count < total_size)
		{
			int error = total_size - count;
			int i = 0;
			for (i = 0; i < error; i++)
			{
				if (use_empty_zero)
					newBuf[i + count] = '0';
				else
					newBuf[i + count] = ' ';
			}
			count += i;
		}
		newBuf[count] = '\0';

		int j = 0;
		for (int i = count - 1; i >= 0; i--)
		{
			buf[j] = newBuf[i];
			j++;
		}
		buf[count] = '\0';
		delete[] newBuf;
	}
};


class ComplexLog;

class ComplexPrintLog
{
	friend class ComplexLog;
public:

	ComplexPrintLog()
	{

	}

	~ComplexPrintLog()
	{

	}

private:

	void ComplexPrintf(const char* format, va_list args)
	{
		ComplexFormat formatting;
		ComplexString test = formatting.GetFormatString(format, args);
		PrintString(test.GetBuffer());
	}

	void PrintString(const char* string)
	{
		while (*string)
		{
			putc(*string, stdout);
			string++;
		}
	}
};

class ComplexFileLog : public ComplexFile
{
	friend class ComplexLog;
public:

	ComplexFileLog()
	{

	}

	~ComplexFileLog()
	{

	}

private:

	void ComplexWrite(ComplexString filePath, const char* format, va_list args)
	{
		ComplexString buf;
		Read(filePath, buf);

		ComplexFormat formatting;
		ComplexString test = formatting.GetFormatString(format, args);
		buf += test;

		if (Write(filePath, buf) == false)
			return;
	}

};

class ComplexLog
{
public:

	ComplexLog()
		: m_printLog(new ComplexPrintLog)
		, m_fileLog(new ComplexFileLog)
	{

	}

	~ComplexLog()
	{
		if (m_printLog)
		{
			delete m_printLog;
			m_printLog = nullptr;
		}

		if (m_fileLog)
		{
			delete m_fileLog;
			m_fileLog = nullptr;
		}
	}

	void FileLog(ComplexString filePath, const char* format, ...)
	{
		va_list args;
		__crt_va_start(args, format);
		m_fileLog->ComplexWrite(filePath, format, args);
		__crt_va_end(args);
	}

	void PrintLog(const char* format, ...)
	{
		va_list args;
		__crt_va_start(args, format);
		m_printLog->ComplexPrintf(format, args);
		__crt_va_end(args);
	}

private:

	ComplexPrintLog* m_printLog;
	ComplexFileLog* m_fileLog;

};
