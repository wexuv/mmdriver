#ifndef __SQLITE3_HANDLER_H__
#define __SQLITE3_HANDLER_H__

#include <sqlite3.h>
#include "driver.h"

namespace driver
{
	class SQLite_Result;

	class SQLite_Handler
	{
	public:

		enum
		{
			sqlite_filename_length = 256,
		};

	private:

		sqlite3* handler_;
		char file_name_[sqlite_filename_length+1];
		bool is_opened_;

	public:
		
		SQLite_Handler (void);
		~SQLite_Handler (void);

		explicit SQLite_Handler (const char* file_name);

		tint32 open (const char* file_name);

		tint32 close (void);

		//用于非SELECT语句(INSERT,UPDATE)
		tint32 execute (const char* sql);

		//用于SELECT语句,得到结果集合的方法
		tint32 execute (const char* sql, SQLite_Result& dbresult);

		//返回最近调用SQLite的API函数的错误代码
		int get_last_errno (void);

	private:

		tint32 open (void);

		SQLite_Handler& operator= (const SQLite_Handler& handler);
		SQLite_Handler (const SQLite_Handler& handler);
	};
}


#endif	//	__SQLITE3_HANDLER_H__


