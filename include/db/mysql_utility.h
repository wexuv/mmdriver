#ifndef __MYSQL_UTILITY_H__
#define __MYSQL_UTILITY_H__

#include "mysql_handler.h"
#include "mysql_result.h"

namespace driver 
{
	class MySQL_Utility
	{
	public:
		enum
		{
			MAX_SQL_STRING_LENGTH = (size_t)0x20000,		// ���SQL���
		};

		MySQL_Utility (void)
		{
			m_szSQL[0] = '\0';
		}

		virtual ~MySQL_Utility (void)
		{
			m_kMysqlHandler.fini();
		}

	protected:

		tint32 _initMySQL(const char* szHostIP, const char* szUser = "root", const char* szPwd = "", const char* szDBName = "");

		tint32 _finiMySQL(void);

		tint32 _selectDB(const char* szDBName);

		tint32 _makeSQLString(const char* szFormat, ...);

		tint32 _makeRealEscapeString (char* pszTo, const char* pszFrom, u_int uiLength);

		inline const char* _getSQLString(void) const
		{
			return m_szSQL;
		}

		inline size_t _getSQLSize(void) const
		{
			return strlen(m_szSQL);
		}

		inline void _clearSQLString(void)
		{
			m_szSQL[0] = '\0';
		}

		//���ڷ�SELECT���(INSERT,UPDATE)
		tint32 _executeSQL (u_int& uiAffectCount, u_int& uiLastID);

		//����SELECT���,ֱ��ת��������ϵķ���
		tint32 _executeSQL (u_int& uiAffectCount, Mysql_Result& rkDBResult);

		//����SELECT���,����use_result�õ�������ϵķ���
		tint32 _executeSQL (Mysql_Result& rkDBResult);

		u_int _getLastMySQLErrno (void);

		const char * _getLastMySQLError(void);

	protected:
		char m_szSQL[MAX_SQL_STRING_LENGTH + 1];
		Mysql_Handler m_kMysqlHandler;
	};

}

#endif // __MYSQL_UTILITY_H__

