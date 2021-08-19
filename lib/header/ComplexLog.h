#pragma once

#include "ComplexFile.h"
#include "ComplexFormat.h"

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
