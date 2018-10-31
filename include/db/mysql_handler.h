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
			MAX_SQL_STRING_LENGTH = (size_t)0x20000,		// 最长的SQL语句
		};

		enum
		{
			mysql_success		= 0,
			mysql_default_port	= 3306,
			mysql_string_length	= 128,
			mysql_dbname_length	= 256,
		};

	private:

		//数据库的IP地址
		char host_ip_[mysql_string_length+1];
		//数据库的用户名
		char account_[mysql_string_length+1];
		//数据库的密码
		char password_[mysql_string_length+1];
		//默认数据库
		char db_name_[mysql_dbname_length+1];
		//数据库的端口号
		u_short port_;

		//sql语句
		char m_szSQL[MAX_SQL_STRING_LENGTH + 1];

		//MYSQL的句柄
		MYSQL handler_;
		//是否连接MYSQL数据库
		bool is_connected_;

	public:

		Mysql_Handler (void);
		~Mysql_Handler (void);

		//初始化DB处理器，会尝试连接一次数据库，即调用connect函数
		tint32 init (const char* host_ip, const char* user = "root", const char* pwd = "", const char* db_name = "", u_short port = mysql_default_port);
	    
		//结束DB处理器，断开与数据库的连接
		tint32 fini (void);

		//选择指定的数据库成为由mysql指定的连接上的默认数据库（当前数据库）
		tint32 select_db (const char* db_name);


		//用于非SELECT语句(INSERT,UPDATE)
		tint32 executeSQL (u_int& uiAffectCount, u_int& uiLastID);

		//用于SELECT语句,直接转储结果集合的方法
		tint32 executeSQL (u_int& uiAffectCount, Mysql_Result& rkDBResult);

		//用于SELECT语句,用于use_result得到结果集合的方法
		tint32 executeSQL (Mysql_Result& rkDBResult);


		//创建可在SQL语句中使用的合法SQL字符串
		tint32 make_real_escape_string (char* to, const char* from, u_int length);

		//生成sql语句
		tint32 make_sql_string(const char* szFormat, ...);

		inline const char* get_sql_string(void) const
		{
			return m_szSQL;
		}

		//返回最近调用MySQL的API函数的错误代码。“0”返回值表示未出现错误，在MySQL errmsg.h头文件中，列出了客户端错误消息编号。
		u_int get_last_errno (void);

		//返回最近调用MySQL的API函数的错误提示
		const char* get_last_error (void); 

	private:

		//连接数据服务器
		tint32 connect (u_int timeout);
	    
		//断开数据服务器
		tint32 disconnect (void);


		//用于非SELECT语句(INSERT,UPDATE)
		tint32 execute (const char* sql, u_int& numaffect, u_int& lastid, size_t sqllen = 0);

		//用于SELECT语句,直接转储结果集合的方法
		tint32 execute (const char* sql, u_int& numaffect, Mysql_Result& dbresult, size_t sqllen = 0);

		//用于SELECT语句,用于use_result得到结果集合的方法
		tint32 execute (const char* sql, Mysql_Result& dbresult, size_t sqllen = 0);

		//excute实现函数，设指针参数的目的在于检查某个参数是否为NULL表示它是否起作用
		tint32 execute (const char* sql, u_int* numaffect, u_int* lastid, Mysql_Result* dbresult, bool bstore, size_t sqllen = 0);

	private:

		Mysql_Handler& operator= (const Mysql_Handler& handler);
		Mysql_Handler (const Mysql_Handler& handler);

	};
}

#endif //__MYSQL_HANDLER_H__



