#include "driver.h"
#include "mysql_handler.h"
#include "mysql_result.h"
#include <string.h>

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

		//�������������ݿ�
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

	//���ڷ�SELECT���(INSERT,UPDATE)
	tint32 Mysql_Handler::execute (const char* sql, u_int& numaffect, u_int& lastid, size_t sqllen)
	{
		return execute(sql, &numaffect, &lastid, NULL, false, sqllen);
	}

	//����SELECT���,ֱ��ת��������ϵķ���
	tint32 Mysql_Handler::execute (const char* sql, u_int& numaffect, Mysql_Result& dbresult, size_t sqllen)
	{
		return execute(sql, &numaffect, NULL, &dbresult, true, sqllen);
	}

	//����SELECT���,����use_result�õ�������ϵķ���
	tint32 Mysql_Handler::execute (const char* sql, Mysql_Result& dbresult, size_t sqllen)
	{
		return execute(sql, NULL, NULL, &dbresult, false, sqllen);
	}

	//excuteʵ�ֺ�������ָ�������Ŀ�����ڼ��ĳ�������Ƿ�ΪNULL��ʾ���Ƿ�������
	tint32 Mysql_Handler::execute (const char* sql, u_int* numaffect, u_int* lastid, Mysql_Result* dbresult, bool bstore, size_t sqllen)
	{
		if(NULL == sql)
		{
			return fail;
		}
	    
		tint32 result = fail;
		//������ݿ�δ�����ϣ�����������һ��
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

		//ִ��SQL����
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

	//�����������MySQL��API�����Ĵ�����롣��0������ֵ��ʾδ���ִ�����MySQL errmsg.hͷ�ļ��У��г��˿ͻ��˴�����Ϣ��š�
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
		//��������Ѿ�����,��ر�ԭ��������
		if(is_connected_)
		{
			disconnect ();
		}

		//��ʼ��MYSQL���
		mysql_init (&handler_);

		//�������ӵĳ�ʱ
		if(0 != timeout)
		{
			mysql_options (&handler_, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)(&timeout));
		}


		my_bool reconnect = 1; 
		mysql_options (&handler_, MYSQL_OPT_RECONNECT, &reconnect);
		//mysql_options (&handler_, MYSQL_SET_CHARSET_NAME, "utf8");


		//u_int client_flag = CLIENT_MULTI_STATEMENTS;
		u_int client_flag = 0;

		//�������ݿ�
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
}

