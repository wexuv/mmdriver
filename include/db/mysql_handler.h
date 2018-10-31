#ifndef __MYSQL_HANDLER_H__
#define __MYSQL_HANDLER_H__

#include "common/types.h"
#include <mysql.h>
#include <mysqld_error.h>

namespace driver
{
	class Mysql_Result;

	class Mysql_Handler
	{
	public:
		enum
		{
			MAX_SQL_STRING_LENGTH = (size_t)0x20000,		// ���SQL���
		};

		enum
		{
			mysql_success		= 0,
			mysql_default_port	= 3306,
			mysql_string_length	= 128,
			mysql_dbname_length	= 256,
		};

	private:

		//���ݿ��IP��ַ
		char host_ip_[mysql_string_length+1];
		//���ݿ���û���
		char account_[mysql_string_length+1];
		//���ݿ������
		char password_[mysql_string_length+1];
		//Ĭ�����ݿ�
		char db_name_[mysql_dbname_length+1];
		//���ݿ�Ķ˿ں�
		u_short port_;

		//sql���
		char m_szSQL[MAX_SQL_STRING_LENGTH + 1];

		//MYSQL�ľ��
		MYSQL handler_;
		//�Ƿ�����MYSQL���ݿ�
		bool is_connected_;

	public:

		Mysql_Handler (void);
		~Mysql_Handler (void);

		//��ʼ��DB���������᳢������һ�����ݿ⣬������connect����
		tint32 init (const char* host_ip, const char* user = "root", const char* pwd = "", const char* db_name = "", u_short port = mysql_default_port);
	    
		//����DB���������Ͽ������ݿ������
		tint32 fini (void);

		//ѡ��ָ�������ݿ��Ϊ��mysqlָ���������ϵ�Ĭ�����ݿ⣨��ǰ���ݿ⣩
		tint32 select_db (const char* db_name);


		//���ڷ�SELECT���(INSERT,UPDATE)
		tint32 executeSQL (u_int& uiAffectCount, u_int& uiLastID);

		//����SELECT���,ֱ��ת��������ϵķ���
		tint32 executeSQL (u_int& uiAffectCount, Mysql_Result& rkDBResult);

		//����SELECT���,����use_result�õ�������ϵķ���
		tint32 executeSQL (Mysql_Result& rkDBResult);


		//��������SQL�����ʹ�õĺϷ�SQL�ַ���
		tint32 make_real_escape_string (char* to, const char* from, u_int length);

		//����sql���
		tint32 make_sql_string(const char* szFormat, ...);

		inline const char* get_sql_string(void) const
		{
			return m_szSQL;
		}

		//�����������MySQL��API�����Ĵ�����롣��0������ֵ��ʾδ���ִ�����MySQL errmsg.hͷ�ļ��У��г��˿ͻ��˴�����Ϣ��š�
		u_int get_last_errno (void);

		//�����������MySQL��API�����Ĵ�����ʾ
		const char* get_last_error (void); 

	private:

		//�������ݷ�����
		tint32 connect (u_int timeout);
	    
		//�Ͽ����ݷ�����
		tint32 disconnect (void);


		//���ڷ�SELECT���(INSERT,UPDATE)
		tint32 execute (const char* sql, u_int& numaffect, u_int& lastid, size_t sqllen = 0);

		//����SELECT���,ֱ��ת��������ϵķ���
		tint32 execute (const char* sql, u_int& numaffect, Mysql_Result& dbresult, size_t sqllen = 0);

		//����SELECT���,����use_result�õ�������ϵķ���
		tint32 execute (const char* sql, Mysql_Result& dbresult, size_t sqllen = 0);

		//excuteʵ�ֺ�������ָ�������Ŀ�����ڼ��ĳ�������Ƿ�ΪNULL��ʾ���Ƿ�������
		tint32 execute (const char* sql, u_int* numaffect, u_int* lastid, Mysql_Result* dbresult, bool bstore, size_t sqllen = 0);

	private:

		Mysql_Handler& operator= (const Mysql_Handler& handler);
		Mysql_Handler (const Mysql_Handler& handler);

	};
}

#endif //__MYSQL_HANDLER_H__



