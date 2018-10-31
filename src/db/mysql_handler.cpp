#include "common/driver.h"
#include "db/mysql_handler.h"
#include "db/mysql_result.h"
#include <string.h>
#include <stdarg.h>

namespace driver
{
	Mysql_Handler::Mysql_Handler (void):
		port_ (mysql_default_port),   
		is_connected_ (false)
	{
		host_ip_[0]  = '\0';
		account_[0]  = '\0';
		password_[0] = '\0';
		db_name_[0]	 = '\0';

		m_szSQL[0] = '\0';
	}

	Mysql_Handler::~Mysql_Handler (void)
	{
		disconnect ();
	}

	tint32 Mysql_Handler::init(const char* host_ip, const char* user, const char* pwd, const char* db_name, u_short port)
	{
		if (NULL == host_ip || NULL == user || NULL == pwd || NULL == db_name)
		{
			return fail;
		}

		strncpy (host_ip_,  host_ip, mysql_string_length);
		strncpy (account_,  user,    mysql_string_length);
		strncpy (password_, pwd,     mysql_string_length);
		strncpy (db_name_,	db_name, mysql_dbname_length);
		port_ = port;

		//尝试着连接数据库
		u_int timeout = 15;
		return connect (timeout);
	}

	tint32 Mysql_Handler::fini (void)
	{
		if (is_connected_)
		{
			disconnect ();
		}

		return success;
	}

	//用于非SELECT语句(INSERT,UPDATE)
	tint32 Mysql_Handler::executeSQL (u_int& uiAffectCount, u_int& uiLastID)
	{
		return execute(m_szSQL, uiAffectCount, uiLastID, strlen(m_szSQL));
	}

	//用于SELECT语句,直接转储结果集合的方法
	tint32 Mysql_Handler::executeSQL (u_int& uiAffectCount, Mysql_Result& rkDBResult)
	{
		return execute(m_szSQL, uiAffectCount, rkDBResult, strlen(m_szSQL));
	}

	//用于SELECT语句,用于use_result得到结果集合的方法
	tint32 Mysql_Handler::executeSQL (Mysql_Result& rkDBResult)
	{
		return execute(m_szSQL, rkDBResult, strlen(m_szSQL));
	}

	tint32 Mysql_Handler::select_db (const char* db_name)
	{
		if (NULL == db_name)
		{
			return fail;
		}
		else
		{
			if (mysql_success == mysql_select_db (&handler_, db_name))
			{
				strncpy(db_name_, db_name, mysql_dbname_length);
				return success;
			}
			else
			{
				return fail;
			}
		}
	}

	tint32 Mysql_Handler::make_real_escape_string (char* to, const char* from, u_int length)
	{
		if (0 < mysql_real_escape_string (&handler_, to, from, length))
		{
			return success;
		}
		else
		{
			return fail;
		}
	}

	tint32 Mysql_Handler::make_sql_string(const char* szFormat, ...)
	{
		va_list stVL;
		va_start(stVL, szFormat);
		tvsnprintf(m_szSQL, MAX_SQL_STRING_LENGTH, szFormat, stVL);
		va_end(stVL);

		m_szSQL[MAX_SQL_STRING_LENGTH] = '\0';
		return success;
	}

	//返回最近调用MySQL的API函数的错误代码。“0”返回值表示未出现错误，在MySQL errmsg.h头文件中，列出了客户端错误消息编号。
	u_int Mysql_Handler::get_last_errno (void)
	{
		return mysql_errno (&handler_);
	}

	const char* Mysql_Handler::get_last_error (void)
	{
		return mysql_error (&handler_);
	}

	tint32 Mysql_Handler::connect (u_int timeout)
	{
		//如果连接已经建立,则关闭原来的连接
		if(is_connected_)
		{
			disconnect ();
		}

		//初始化MYSQL句柄
		mysql_init (&handler_);

		//设置连接的超时
		if(0 != timeout)
		{
			mysql_options (&handler_, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)(&timeout));
		}


		my_bool reconnect = 1; 
		mysql_options (&handler_, MYSQL_OPT_RECONNECT, &reconnect);
		//mysql_options (&handler_, MYSQL_SET_CHARSET_NAME, "utf8");


		//u_int client_flag = CLIENT_MULTI_STATEMENTS;
		u_int client_flag = 0;

		//连接数据库
		MYSQL* result = NULL;
		result = mysql_real_connect (&handler_, host_ip_, account_, password_, db_name_, port_, NULL, client_flag);
		if (NULL == result)
		{
			return fail;
		}
	    
		is_connected_ = true;
		return success;
	}


	tint32 Mysql_Handler::disconnect (void)
	{
		if (!is_connected_)
		{
			return success;
		}

		mysql_close (&handler_);
		is_connected_ = false;
		return success;
	}

	//用于非SELECT语句(INSERT,UPDATE)
	tint32 Mysql_Handler::execute (const char* sql, u_int& numaffect, u_int& lastid, size_t sqllen)
	{
		return execute(sql, &numaffect, &lastid, NULL, false, sqllen);
	}

	//用于SELECT语句,直接转储结果集合的方法
	tint32 Mysql_Handler::execute (const char* sql, u_int& numaffect, Mysql_Result& dbresult, size_t sqllen)
	{
		return execute(sql, &numaffect, NULL, &dbresult, true, sqllen);
	}

	//用于SELECT语句,用于use_result得到结果集合的方法
	tint32 Mysql_Handler::execute (const char* sql, Mysql_Result& dbresult, size_t sqllen)
	{
		return execute(sql, NULL, NULL, &dbresult, false, sqllen);
	}

	//excute实现函数，设指针参数的目的在于检查某个参数是否为NULL表示它是否起作用
	tint32 Mysql_Handler::execute (const char* sql, u_int* numaffect, u_int* lastid, Mysql_Result* dbresult, bool bstore, size_t sqllen)
	{
		if(NULL == sql)
		{
			return fail;
		}

		tint32 result = fail;
		//如果数据库未连接上，则尝试着连接一次
		if(!is_connected_)
		{
			u_int timeout = 1;
			result = connect (timeout);
			if (success != result)
			{
				return fail;
			}

		}
		else
		{
			result = mysql_ping (&handler_);
			if (mysql_success != result)
			{
				return fail;
			}
		}

		//执行SQL命令
		result = mysql_real_query (&handler_, sql, (unsigned long)sqllen);
		if (mysql_success != result)
		{
			return fail;
		}

		if(NULL != dbresult)
		{
			MYSQL_RES* mysql_result = NULL;
			if(bstore)
			{
				mysql_result = mysql_store_result (&handler_);
			}
			else
			{
				mysql_result = mysql_use_result (&handler_);
			}

			if((NULL == mysql_result) && (mysql_field_count (&handler_) > 0))
			{
				return fail;
			}

			dbresult->set_result (mysql_result);
		}

		if(NULL != numaffect)
		{
			*numaffect = static_cast<u_int>(mysql_affected_rows (&handler_)) ;
		}
		if(NULL != lastid)
		{
			*lastid = static_cast<u_int>(mysql_insert_id (&handler_)) ;
		}

		return success;
	}
}

