#pragma once

#include "ComplexString.h"
#include "ComplexMap.h"
#include "sqlite3.h"

/*
* Query�� �����ۼ� ���
* ���� �������� ������ �ڿ� ;�� ������ �ʴ´�.
* ���� �������� ������ ���� �ڿ� ;�� ������ �����Ѵ�.
* �������� Į���� �� ���� �����ϰ� �빮�ڷ� �ۼ��Ѵ�.
* ���ڿ��� \" �̳� ' �� �Է��Ѵ�.

DDL ��ɾ� (CREATE, ALTER DROP)
CREATE TABLE table_name (column_name column_type(size), ...);
ALTER TABLE table_name ...;
-> (...���� ADD, ALTER, DROP �Է°���)
-> ADD column_name column_type(size)
-> ALTER column_name column_type(size)
-> DROP column_name
DROP TABLE table_name;

DML ��ɾ� (SELECT, INSERT, UPDATE, DELETE)
SELECT ...(1) FROM table_name WHERE ...(2);
-> ...(1)���� �˻��ϰ� ���� Į���� �Է� (* �� ��� Į�� �˻�)
-> ...(2)���� Ư�� Ʃ�ø� �˻��ϰ� ���� Į����=�� �Է�
INSERT INTO table_name (column_name, ...) VALUES(column_value);
-> INTO ���� ��ȣ�� ���� �����ϸ� �������ϸ� �Է��ϰ� ���� Į���� Ư���ؼ� �Է��Ѵ�.
-> �������ϸ� VALUES ���� �� �Է� ���� ���� �������� �Է� �ؾ��Ѵ�.
-> �Է� ���� VALUE�� NULL������ ���� notnull�� VALUE�� ���� �Ұ����ϴ�.
UPDATE table_name SET ...;
-> ...���� Ư�� Į���� �������� ������ �Է��Ѵ�.
DELETE FROM table_name WHERE ...;
-> ...���� Ư�� Į���� �������� ������ �Է��Ѵ�.

*/

//#define SQLITE_OK           0   /* Successful result */
/* beginning-of-error-codes */
//#define SQLITE_ERROR        1   /* SQL error or missing database */
//#define SQLITE_INTERNAL     2   /* Internal logic error in SQLite */
//#define SQLITE_PERM         3   /* Access permission denied */
//#define SQLITE_ABORT        4   /* Callback routine requested an abort */
//#define SQLITE_BUSY         5   /* The database file is locked */
//#define SQLITE_LOCKED       6   /* A table in the database is locked */
//#define SQLITE_NOMEM        7   /* A malloc() failed */
//#define SQLITE_READONLY     8   /* Attempt to write a readonly database */
//#define SQLITE_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
//#define SQLITE_IOERR       10   /* Some kind of disk I/O error occurred */
//#define SQLITE_CORRUPT     11   /* The database disk image is malformed */
//#define SQLITE_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
//#define SQLITE_FULL        13   /* Insertion failed because database is full */
//#define SQLITE_CANTOPEN    14   /* Unable to open the database file */
//#define SQLITE_PROTOCOL    15   /* Database lock protocol error */
//#define SQLITE_EMPTY       16   /* Database is empty */
//#define SQLITE_SCHEMA      17   /* The database schema changed */
//#define SQLITE_TOOBIG      18   /* String or BLOB exceeds size limit */
//#define SQLITE_CONSTRAINT  19   /* Abort due to constraint violation */
//#define SQLITE_MISMATCH    20   /* Data type mismatch */
//#define SQLITE_MISUSE      21   /* Library used incorrectly */
//#define SQLITE_NOLFS       22   /* Uses OS features not supported on host */
//#define SQLITE_AUTH        23   /* Authorization denied */
//#define SQLITE_FORMAT      24   /* Auxiliary database format error */
//#define SQLITE_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
//#define SQLITE_NOTADB      26   /* File opened that is not a database file */
//#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
//#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

namespace ComplexLibrary
{
	class ComplexDatabase
	{
	public:

		ComplexString GetErrorCode(int code)
		{
			ComplexString error = "";
			switch (code)
			{
			case SQLITE_OK:
				error = "success.";
				break;
			case SQLITE_ERROR:
				error = "error!, not found error code.";
				break;
			case SQLITE_INTERNAL:
				error = "find bug in sqlite.";
				break;
			case SQLITE_PERM:
				error = "access deny.";
				break;
			case SQLITE_ABORT:
				error = "sqlite3_exec() callback_func not return 0.";
				break;
			case SQLITE_BUSY:
				error = "db file is locked.";
				break;
			case SQLITE_LOCKED:
				error = "db table is locked.";
				break;
			case SQLITE_NOMEM:
				error = "sqlite full memory";
				break;
			case SQLITE_READONLY:
				error = "have not write grant.";
				break;
			case SQLITE_IOERR:
				error = "os detected I/O error.";
				break;
			case SQLITE_CORRUPT:
				error = "db file is damaged.";
				break;
			case SQLITE_FULL:
				error = "disk is full.";
				break;
			case SQLITE_CANTOPEN:
				error = "can not open db file.";
				break;
			case SQLITE_SCHEMA:
				error = "en route processing 'sqlite3_step()', schema changed.";
				break;
			case SQLITE_TOOBIG:
				error = "size of String and BLOB is exceed.";
				break;
			case SQLITE_CONSTRAINT:
				error = "query is violated a constraint.";
				break;
			case SQLITE_MISMATCH:
				error = "data type is mismatched.";
				break;
			case SQLITE_RANGE:
				error = "row index of parameter of sqlite3_bind() and sqlite3_coloumn() out of range.";
				break;
			case SQLITE_NOTADB:
				error = "file is not db file.";
				break;
			case SQLITE_ROW:
				error = "result of sqlite3_step() indicates that it can be returned.";
				break;
			case SQLITE_DONE:
				error = "sqlite3_setp() is processed.";
				break;
			default:
				break;
			}

			return error;
		}

		ComplexDatabase()
			: m_bDBConnection(false)
			, m_bPrepareStatement(false)
			, m_bResultSet(false)
			, m_bUseAutoCommit(true)
			, m_nCommitStartCount(0)
		{

		}

		~ComplexDatabase()
		{
			DisConnectDatabase();
		}

		/*
		* @ brief db�� �����Ѵ�.
		* @ param 1 : ���� �ؽ�Ʈ
		* @ return ���� ���� ����
		*/
		bool ConnectDatabase(ComplexString fileName, ComplexString* error = NULL)
		{
			int isOpen = sqlite3_open(fileName.GetBuffer(), &m_dbHandler);
			if (error != NULL)
				*error = GetErrorCode(isOpen);
			if (isOpen != SQLITE_OK)
			{
				sqlite3_close(m_dbHandler);

				return false;
			}

			m_bDBConnection = true;

			return true;
		}

		/*
		* @ brief db ���Ῡ�θ� Ȯ���Ѵ�.
		* @ return ���� ���� ����
		*/
		bool IsConnect() const
		{
			return m_bDBConnection;
		}

		/*
		* @ brief ����� db�� �����Ѵ�.
		* @ param 1 : ���� �ؽ�Ʈ
		* @ return ���� ���� ����
		*/
		bool DisConnectDatabase(ComplexString* error = NULL)
		{
			if (m_bDBConnection)
			{
				int errorcode = sqlite3_close(m_dbHandler);
				m_bDBConnection = false;
				if (error != NULL)
					*error = GetErrorCode(errorcode);
				return true;
			}

			return false;
		}

		/*
		* @ brief select�� ���๮
		* prepare query�� �ƴϾ ����ȴ�.
		* @ param 1 : select �� ������ �ۼ� (�������� ���� �빮�ڷ�)
		*         2 : preparestatement�� ? ���� ä�� �ܺ��Լ� �Ķ���� (��� �ۼ��Ͽ� ����)
		*         3 : resultset�� ������� ���� �ܺ��Լ� �Ķ���� (��� �ۼ��Ͽ� ����) (���� Ʃ���̸� �ݺ������� ȣ���)
		*		  4 : ���� �ؽ�Ʈ
		* @ return ������ ���� ����
		*/
		bool PrepareStatement_Execute(ComplexString query, void(*SetPrepareStatement)(), void(*ResultSet)(), ComplexString* error = NULL)
		{
			if (!m_bDBConnection)
				return false;

			if (query.Find("SELECT") == -1)
				return false;

			if (!m_bUseAutoCommit)
			{
				if (m_nCommitStartCount == 0)
					sqlite3_exec(m_dbHandler, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);
			}

			int result = sqlite3_prepare_v2(m_dbHandler, query.GetBuffer(), -1, &m_queryHandler, 0);
			if (result == SQLITE_OK)
			{
				m_bPrepareStatement = true;
				if (SetPrepareStatement != NULL)
					SetPrepareStatement();
				m_bResultSet = true;

				while (sqlite3_step(m_queryHandler) == SQLITE_ROW)
				{
					if (ResultSet != NULL)
						ResultSet();
				}

				sqlite3_finalize(m_queryHandler);
				m_queryHandler = NULL;
				m_bPrepareStatement = false;
				m_bResultSet = false;
			}
			if (error != NULL)
				*error = GetErrorCode(result);

			return true;
		}

		/*
		* @ brief not preparestatement ������ ����
		* @ param 1 : ��� ������ �ۼ� (�������� ���� �빮�ڷ�)
		*         2 : sqlite3_callback �Լ��� �ۼ��Ͽ� ���� (���� �������� ������)
		*		  3 : ���� �ؽ�Ʈ
		* @ �ݹ��Լ� ������
		* static int �Լ���(void* notUse, int arg_count, char** arg_value, char** azColName)
		* ���� ������ �������� Ʃ���� ��� �ش� �ݹ��Լ��� ������ ȣ���
		* @ return ������ ���� ����
		*/
		bool ExecuteQuery(ComplexString query, sqlite3_callback query_callback, ComplexString* error = NULL)
		{
			if (!m_bDBConnection)
				return false;

			int result = sqlite3_exec(m_dbHandler, query, query_callback, 0, 0);
			if (error != NULL)
				*error = GetErrorCode(result);

			return true;
		}

		/*
		* @ brief insert, update, delete�� ���๮
		* prepare query�� �ƴϾ ����ȴ�.
		* @ param 1 : insert, update, delete �� ������ �ۼ� (�������� ���� �빮�ڷ�)
		*         2 : preparestatement�� ? ���� ä�� �ܺ��Լ� �Ķ���� (��� �ۼ��Ͽ� ����)
		*		  3 : ���� �ؽ�Ʈ
		* @ return ������ ���� ����
		*/
		bool PrepareStatement_Execute(ComplexString query, void(*SetPrepareStatement)(), ComplexString* error = NULL)
		{
			if (!m_bDBConnection)
				return false;

			if (!m_bUseAutoCommit)
			{
				if (m_nCommitStartCount == 0)
					sqlite3_exec(m_dbHandler, "BEGIN IMMEDIATE TRANSACTION;", NULL, NULL, NULL);
			}

			if (query.Find("INSERT") == -1 && query.Find("UPDATE") == -1 && query.Find("DELETE") == -1)
				return false;

			int result = sqlite3_prepare_v2(m_dbHandler, query.GetBuffer(), -1, &m_queryHandler, 0);
			if (result == SQLITE_OK)
			{
				m_bPrepareStatement = true;
				if (SetPrepareStatement != NULL)
					SetPrepareStatement();
				sqlite3_step(m_queryHandler);

				sqlite3_finalize(m_queryHandler);
				m_queryHandler = NULL;
				m_bPrepareStatement = false;
			}
			if (error != NULL)
				*error = GetErrorCode(result);
			return true;
		}

		/*
		* @ brief preparestatement�� ? ���� ä��� ���� �Լ� (�� SetPrepareStatement�� �ܺ��Լ��� �ۼ�����)
		* @ param 1 : ? ��ġ�� �ε�����ȣ 1���� ����
		*         2 : ? �� �� ('�̳� \" �� ���������� ����)
		*		  3 : ���� �ؽ�Ʈ
		*/
		void SetString(int prepare_index, ComplexString value, ComplexString* error = NULL)
		{
			if (!m_bPrepareStatement)
				return;

			int result = sqlite3_bind_text(m_queryHandler, prepare_index, value.GetBuffer(), -1, SQLITE_TRANSIENT);	// SQLITE_TRANSIENT �� ��°�� �����Ͽ� ����. / SQLITE_STATIC �� �ּҰ��� �����ؼ� ����
			if (error != NULL)
				*error = GetErrorCode(result);
		}

		/*
		* @ brief preparestatement�� ? ���� ä��� ���� �Լ� (�� SetPrepareStatement�� �ܺ��Լ��� �ۼ�����)
		* @ param 1 : ? ��ġ�� �ε�����ȣ 1���� ����
		*         2 : ? �� ��
		*		  3 : ���� �ؽ�Ʈ
		*/
		void SetInt(int prepare_index, int value, ComplexString* error = NULL)
		{
			if (!m_bPrepareStatement)
				return;

			int result = sqlite3_bind_int(m_queryHandler, prepare_index, value);
			if (error != NULL)
				*error = GetErrorCode(result);
		}

		/*
		* @ brief preparestatement�� ? ���� ä��� ���� �Լ� (�� SetPrepareStatement�� �ܺ��Լ��� �ۼ�����)
		* @ param 1 : ? ��ġ�� �ε�����ȣ 1���� ����
		*         2 : ? �� ��
		* @		  3 : ���� �ؽ�Ʈ
		*/
		void SetDouble(int prepare_index, double value, ComplexString* error = NULL)
		{
			if (!m_bPrepareStatement)
				return;

			int result = sqlite3_bind_double(m_queryHandler, prepare_index, value);
			if (error != NULL)
				*error = GetErrorCode(result);
		}

		/*
		* @ brief resultset�� ������� �����Ѵ�.
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
		* @ brief resultset�� ������� �����Ѵ�.
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
		* @ brief resultset�� ������� �����Ѵ�.
		* @ param 1 : Į���� �ε�����ȣ 0���� ����
		* @ return �����
		*/
		double GetDouble(int column_index)
		{
			if (!m_bResultSet)
				return -1;

			return sqlite3_column_double(m_queryHandler, column_index);
		}

		/*
		* @ brief ���� Ŀ���� �����Ѵ�.
		* @ param 1 : true = ����Ŀ�� ����, false = ����Ŀ�� ������
		*/
		void SetAutoCommit(bool useAutoCommit)
		{
			m_bUseAutoCommit = useAutoCommit;
		}

		/*
		* @ brief ����Ŀ�� �������� �� ����. ���ݱ��� ������ �������� Ŀ���Ѵ�.
		* @ param 1 : ���� �ؽ�Ʈ
		*/
		void Commit(ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				int result = sqlite3_exec(m_dbHandler, "COMMIT TRANSACTION;", NULL, NULL, NULL);
				if (error != NULL)
					*error = GetErrorCode(result);
				m_nCommitStartCount = 0;
			}
		}

		/*
		* @ brief ����Ŀ�� �������� �� ����. ���ݱ��� ������ �������� �ѹ��Ѵ�. (���̺� ����Ʈ��)
		* @ param 1 : �ѹ��� ���̺� ����Ʈ �̸�. (���� �Է����� ������ �ʱ�� ���ư���.)
		* @ param 2 : ���� �ؽ�Ʈ
		*/
		void Rollback(ComplexString savepoint_name = "", ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				int result = -1;
				if (savepoint_name.IsEmpty())
				{
					result = sqlite3_exec(m_dbHandler, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
				}
				else
				{
					ComplexString query;
					query.Format("ROLLBACK TO %s;", savepoint_name.GetBuffer());
					result = sqlite3_exec(m_dbHandler, query, NULL, NULL, NULL);
				}
				if (error != NULL)
					*error = GetErrorCode(result);
				m_nCommitStartCount = 0;
			}
		}

		/*
		* @ brief ����Ŀ�� �������� �� ����. ���̺� ����Ʈ�� �����Ѵ�.
		* @ param 1 : ������ ���̺� ����Ʈ �̸�.
		* @ param 2 : ���� �ؽ�Ʈ
		*/
		void SetSavePoint(ComplexString savepoint_name, ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				ComplexString query;
				query.Format("SAVEPOINT %s;", savepoint_name.GetBuffer());
				int result = sqlite3_exec(m_dbHandler, query.GetBuffer(), NULL, NULL, NULL);
				if (error != NULL)
					*error = GetErrorCode(result);
			}
		}

		/*
		* @ brief ����Ŀ�� �������� �� ����. ������ ���̺� ����Ʈ�� �����Ѵ�.
		* @ param 1 : ������ ���̺� ����Ʈ �̸�.
		* @ param 2 : ���� �ؽ�Ʈ
		*/
		void ReleaseSavePoint(ComplexString savepoint_name, ComplexString* error = NULL)
		{
			if (!m_bUseAutoCommit)
			{
				ComplexString query;
				query.Format("RELEASE %s;", savepoint_name.GetBuffer());
				int result = sqlite3_exec(m_dbHandler, query.GetBuffer(), NULL, NULL, NULL);
				if (error != NULL)
					*error = GetErrorCode(result);
			}
		}

	private:

		sqlite3*		m_dbHandler;			// �����ͺ��̽� �ڵ�
		sqlite3_stmt*	m_queryHandler;			// ������ �ڵ�

		bool			m_bDBConnection;		// db ���� ����
		bool			m_bPrepareStatement;	// preparestatement ��� ����
		bool			m_bResultSet;			// resultset ��� ����

		bool			m_bUseAutoCommit;		// autocommit ��� ����
		int				m_nCommitStartCount;	// autocommit ��� ���� �� "BEGIN"�� �ۼ��� ī��Ʈ ��ȣ (0���϶��� �ۼ���, ROLLBACK�̳� COMMIT�ϸ� 0���� �ʱ�ȭ)

	};
}