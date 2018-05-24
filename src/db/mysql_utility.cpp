#include "mysql_utility.h"
#include "driver.h"
#include <stdarg.h>

namespace driver 
{
	tint32 MySQL_Utility::_initMySQL(const char* szHostIP, const char* szUser, const char* szPwd, const char* szDBName)
	{
		return m_kMysqlHandler.init(szHostIP, szUser, szPwd, szDBName);
	}

	tint32 MySQL_Utility::_finiMySQL(void)
	{
		return m_kMysqlHandler.fini();
	}

	tint32 MySQL_Utility::_selectDB(const char* szDBName)
	{
		return m_kMysqlHandler.select_db(szDBName);
	}

	tint32 MySQL_Utility::_makeSQLString(const char* szFormat, ...)
	{
		va_list stVL;
		va_start(stVL, szFormat);
		vsnprintf(m_szSQL, MAX_SQL_STRING_LENGTH, szFormat, stVL);
		va_end(stVL);

		m_szSQL[MAX_SQL_STRING_LENGTH] = '\0';
		return success;
	}

	tint32 MySQL_Utility::_makeRealEscapeString (char* pszTo, const char* pszFrom, u_int uiLength)
	{
		return m_kMysqlHandler.make_real_escape_string(pszTo, pszFrom, uiLength);
	}

	//用于非SELECT语句(INSERT,UPDATE)
	tint32 MySQL_Utility::_executeSQL (u_int& uiAffectCount, u_int& uiLastID)
	{
		return m_kMysqlHandler.execute(m_szSQL, uiAffectCount, uiLastID, strlen(m_szSQL));
	}

	//用于SELECT语句,直接转储结果集合的方法
	tint32 MySQL_Utility::_executeSQL (u_int& uiAffectCount, Mysql_Result& rkDBResult)
	{
		return m_kMysqlHandler.execute(m_szSQL, uiAffectCount, rkDBResult, strlen(m_szSQL));
	}

	//用于SELECT语句,用于use_result得到结果集合的方法
	tint32 MySQL_Utility::_executeSQL (Mysql_Result& rkDBResult)
	{
		return m_kMysqlHandler.execute(m_szSQL, rkDBResult, strlen(m_szSQL));
	}

	u_int MySQL_Utility::_getLastMySQLErrno (void)
	{
		return m_kMysqlHandler.get_last_errno();
	}

	const char * MySQL_Utility::_getLastMySQLError(void)
	{
		return m_kMysqlHandler.get_last_error();
	}

}


