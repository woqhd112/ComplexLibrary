#pragma once

#include "ComplexString.h"
#include "ComplexMap.h"
#include "sqlite3.h"

class ComplexDatabase
{
public:

	ComplexDatabase()
		: m_bDBConnection(false)
		, m_bPrepareStatement(false)
		, m_bResultSet(false)
	{
	
	}

	~ComplexDatabase()
	{
		DisConnectDatabase();
	}

	bool ConnectDatabase(ComplexString fileName)
	{
		int isOpen = sqlite3_open(fileName.GetBuffer(), &m_dbHandler);

		if (isOpen != SQLITE_OK)
		{
			sqlite3_close(m_dbHandler);

			return false;
		}

		m_bDBConnection = true;

		return true;
	}

	bool DisConnectDatabase()
	{
		if (m_bDBConnection)
		{
			sqlite3_close(m_dbHandler);
			return true;
		}

		return false;
	}

	/*
	* @ brief select절 실행문
	* @ param 1 : select 절 쿼리문 작성 (쿼리문은 전부 대문자로)
	*         2 : preparestatement의 ? 값을 채울 외부함수 파라미터 (모두 작성하여 전송)
	*         3 : resultset의 결과값을 받을 외부함수 파라미터 (모두 작성하여 전송)
	* @ return 쿼리문 성공 여부
	*/
	bool ExecuteQuery(ComplexString query, void (*SetPrepareStatement)(), void (*ResultSet)())
	{
		if (!m_bDBConnection)
			return false;

		if (query.Find("SELECT") == -1)
			return false;

		if (sqlite3_prepare_v2(m_dbHandler, query.GetBuffer(), -1, &m_queryHandler, 0) == SQLITE_OK)
		{
			m_bPrepareStatement = true;
			SetPrepareStatement();
			m_bResultSet = true;
			ResultSet();

			sqlite3_finalize(m_queryHandler);
			m_queryHandler = NULL;
			m_bPrepareStatement = false;
			m_bResultSet = false;
		}
	}

	/*
	* @ brief insert, update, delete절 실행문
	* @ param 1 : insert, update, delete 절 쿼리문 작성 (쿼리문은 전부 대문자로)
	*         2 : preparestatement의 ? 값을 채울 외부함수 파라미터 (모두 작성하여 전송)
	* @ return 쿼리문 성공 여부
	*/
	bool Execute(ComplexString query, void(*SetPrepareStatement)())
	{
		if (!m_bDBConnection)
			return false;

		if (query.Find("INSERT") == -1 && query.Find("UPDATE") == -1 && query.Find("DELETE") == -1)
			return false;

		if (sqlite3_prepare_v2(m_dbHandler, query.GetBuffer(), -1, &m_queryHandler, 0) == SQLITE_OK)
		{
			m_bPrepareStatement = true;
			SetPrepareStatement();

			sqlite3_finalize(m_queryHandler);
			m_queryHandler = NULL;
			m_bPrepareStatement = false;
		}
	}

	/*
	* @ brief preparestatement의 ? 값을 채우기 위한 함수 (꼭 SetPrepareStatement의 외부함수에 작성하자)
	* @ param 1 : ? 위치의 인덱스번호 1부터 시작
	*         2 : ? 의 값 (String 값이면 ''를 붙여서 전송하자)
	*/
	void SetString(int prepare_index, ComplexString value)
	{
		if (!m_bPrepareStatement)
			return;

		sqlite3_bind_text(m_queryHandler, prepare_index, value.GetBuffer(), -1, 0);
	}

	/*
	* @ brief preparestatement의 ? 값을 채우기 위한 함수 (꼭 SetPrepareStatement의 외부함수에 작성하자)
	* @ param 1 : ? 위치의 인덱스번호 1부터 시작
	*         2 : ? 의 값
	*/
	void SetInt(int prepare_index, int value)
	{
		if (!m_bPrepareStatement)
			return;

		sqlite3_bind_int(m_queryHandler, prepare_index, value);
	}

	/*
	* @ brief preparestatement의 ? 값을 채우기 위한 함수 (꼭 SetPrepareStatement의 외부함수에 작성하자)
	* @ param 1 : ? 위치의 인덱스번호 1부터 시작
	*         2 : ? 의 값
	*/
	void SetDouble(int prepare_index, double value)
	{
		if (!m_bPrepareStatement)
			return;

		sqlite3_bind_double(m_queryHandler, prepare_index, value);
	}

	/*
	* @ brief resultset의 결과값이 존재하는지 여부 (꼭 ResultSet의 외부함수에서 while문 조건에 넣자)
	*/
	bool NextResult()
	{
		if (!m_bResultSet)
			return false;

		return sqlite3_step(m_queryHandler) == SQLITE_ROW;
	}

	/*
	* @ brief resultset의 결과값을 리턴한다. (꼭 ResultSet의 외부함수에서 while문 안에 넣자)
	* @ param 1 : 칼럼의 인덱스번호 0부터 시작
	* @ return 결과값
	*/
	ComplexString GetString(int column_index)
	{
		if (!m_bResultSet)
			return "";

		return ComplexString((const char*)sqlite3_column_text(m_queryHandler, column_index));
	}

	/*
	* @ brief resultset의 결과값을 리턴한다. (꼭 ResultSet의 외부함수에서 while문 안에 넣자)
	* @ param 1 : 칼럼의 인덱스번호 0부터 시작
	* @ return 결과값
	*/
	int GetInt(int column_index)
	{
		if (!m_bResultSet)
			return -1;

		return sqlite3_column_int(m_queryHandler, column_index);
	}

	/*
	* @ brief resultset의 결과값을 리턴한다. (꼭 ResultSet의 외부함수에서 while문 안에 넣자)
	* @ param 1 : 칼럼의 인덱스번호 0부터 시작
	* @ return 결과값
	*/
	double GetDouble(int column_index)
	{
		if (!m_bResultSet)
			return -1;

		return sqlite3_column_double(m_queryHandler, column_index);
	}

private:

	sqlite3* m_dbHandler;
	sqlite3_stmt* m_queryHandler;

	bool m_bDBConnection;
	bool m_bPrepareStatement;
	bool m_bResultSet;

};