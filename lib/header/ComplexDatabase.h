#pragma once

#include "ComplexString.h"
#include "sqlite3.h"

class ComplexDatabase
{
public:

	ComplexDatabase()
	{
	
	}

	~ComplexDatabase()
	{
	
	}

	bool ConnectDatabase(ComplexString fileName)
	{
		int isOpen = sqlite3_open(fileName.GetBuffer(), &m_dbHandler);
	}

private:

	sqlite3* m_dbHandler;
	sqlite3_stmt* m_queryHandler;

};