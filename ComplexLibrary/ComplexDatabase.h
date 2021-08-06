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
	* @ brief select�� ���๮
	* @ param 1 : select �� ������ �ۼ� (�������� ���� �빮�ڷ�)
	*         2 : preparestatement�� ? ���� ä�� �ܺ��Լ� �Ķ���� (��� �ۼ��Ͽ� ����)
	*         3 : resultset�� ������� ���� �ܺ��Լ� �Ķ���� (��� �ۼ��Ͽ� ����)
	* @ return ������ ���� ����
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
	* @ brief insert, update, delete�� ���๮
	* @ param 1 : insert, update, delete �� ������ �ۼ� (�������� ���� �빮�ڷ�)
	*         2 : preparestatement�� ? ���� ä�� �ܺ��Լ� �Ķ���� (��� �ۼ��Ͽ� ����)
	* @ return ������ ���� ����
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
	* @ brief preparestatement�� ? ���� ä��� ���� �Լ� (�� SetPrepareStatement�� �ܺ��Լ��� �ۼ�����)
	* @ param 1 : ? ��ġ�� �ε�����ȣ 1���� ����
	*         2 : ? �� �� (String ���̸� ''�� �ٿ��� ��������)
	*/
	void SetString(int prepare_index, ComplexString value)
	{
		if (!m_bPrepareStatement)
			return;

		sqlite3_bind_text(m_queryHandler, prepare_index, value.GetBuffer(), -1, 0);
	}

	/*
	* @ brief preparestatement�� ? ���� ä��� ���� �Լ� (�� SetPrepareStatement�� �ܺ��Լ��� �ۼ�����)
	* @ param 1 : ? ��ġ�� �ε�����ȣ 1���� ����
	*         2 : ? �� ��
	*/
	void SetInt(int prepare_index, int value)
	{
		if (!m_bPrepareStatement)
			return;

		sqlite3_bind_int(m_queryHandler, prepare_index, value);
	}

	/*
	* @ brief preparestatement�� ? ���� ä��� ���� �Լ� (�� SetPrepareStatement�� �ܺ��Լ��� �ۼ�����)
	* @ param 1 : ? ��ġ�� �ε�����ȣ 1���� ����
	*         2 : ? �� ��
	*/
	void SetDouble(int prepare_index, double value)
	{
		if (!m_bPrepareStatement)
			return;

		sqlite3_bind_double(m_queryHandler, prepare_index, value);
	}

	/*
	* @ brief resultset�� ������� �����ϴ��� ���� (�� ResultSet�� �ܺ��Լ����� while�� ���ǿ� ����)
	*/
	bool NextResult()
	{
		if (!m_bResultSet)
			return false;

		return sqlite3_step(m_queryHandler) == SQLITE_ROW;
	}

	/*
	* @ brief resultset�� ������� �����Ѵ�. (�� ResultSet�� �ܺ��Լ����� while�� �ȿ� ����)
	* @ param 1 : Į���� �ε�����ȣ 0���� ����
	* @ return �����
	*/
	ComplexString GetString(int column_index)
	{
		if (!m_bResultSet)
			return "";

		return ComplexString((const char*)sqlite3_column_text(m_queryHandler, column_index));
	}

	/*
	* @ brief resultset�� ������� �����Ѵ�. (�� ResultSet�� �ܺ��Լ����� while�� �ȿ� ����)
	* @ param 1 : Į���� �ε�����ȣ 0���� ����
	* @ return �����
	*/
	int GetInt(int column_index)
	{
		if (!m_bResultSet)
			return -1;

		return sqlite3_column_int(m_queryHandler, column_index);
	}

	/*
	* @ brief resultset�� ������� �����Ѵ�. (�� ResultSet�� �ܺ��Լ����� while�� �ȿ� ����)
	* @ param 1 : Į���� �ε�����ȣ 0���� ����
	* @ return �����
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